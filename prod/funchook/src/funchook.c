/* -*- indent-tabs-mode: nil -*-
 *
 * This file is part of Funchook.
 * https://github.com/kubo/funchook
 *
 * Funchook is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 2 of the License, or (at your
 * option) any later version.
 *
 * As a special exception, the copyright holders of this library give you
 * permission to link this library with independent modules to produce an
 * executable, regardless of the license terms of these independent
 * modules, and to copy and distribute the resulting executable under
 * terms of your choice, provided that you also meet, for each linked
 * independent module, the terms and conditions of the license of that
 * module. An independent module is a module which is not derived from or
 * based on this library. If you modify this library, you may extend this
 * exception to your version of the library, but you are not obliged to
 * do so. If you do not wish to do so, delete this exception statement
 * from your version.
 *
 * Funchook is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 * for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Funchook. If not, see <http://www.gnu.org/licenses/>.
 */
#include "config.h"
#include <stdio.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <limits.h>
#ifdef WIN32
#include <windows.h>
#ifndef PATH_MAX
#define PATH_MAX MAX_PATH
#endif
#endif
#include "funchook.h"
#include "funchook_internal.h"

#define FUNCHOOK_MAX_ERROR_MESSAGE_LEN 200

#define BOOST_PP_STRINGIZE(text) BOOST_PP_STRINGIZE_I(text)
#define BOOST_PP_STRINGIZE_I(text) #text

const char *LIB_INFO = "SAP_DAE_VERSION: " BOOST_PP_STRINGIZE(VERSION_MAJOR) "." BOOST_PP_STRINGIZE(VERSION_MINOR) "." BOOST_PP_STRINGIZE(BUILD_NUMBER);

struct funchook_page {
    struct funchook_page *next;
    uint16_t used;
    funchook_entry_t entries[1];
};

struct funchook {
    int installed;
    funchook_page_t *page_list;
    char error_message[FUNCHOOK_MAX_ERROR_MESSAGE_LEN];
    FILE *fp;
};

char funchook_debug_file[PATH_MAX];

const size_t funchook_size = sizeof(funchook_t);

static size_t num_entries_in_page;

static void funchook_logv(funchook_t *funchook, int set_error, const char *fmt, va_list ap);
static void funchook_log_end(funchook_t *funchook, const char *fmt, ...);
static funchook_t *funchook_create_internal(void);
static int funchook_prepare_internal(funchook_t *funchook, void **target_func, void *hook_func);
static int funchook_install_internal(funchook_t *funchook, int flags);
static int funchook_uninstall_internal(funchook_t *funchook, int flags);
static int funchook_destroy_internal(funchook_t *funchook);
static int get_page(funchook_t *funchook, funchook_page_t **page_out, uint8_t *addr, rip_displacement_t *disp);

int BeginWith_TOC_Code(const uint8_t* p);

#ifdef CPU_X86_64

int funchook_page_avail(funchook_t *funchook, funchook_page_t *page, int idx, uint8_t *addr, rip_displacement_t *disp)
{
    funchook_entry_t *entry = &page->entries[idx];
    const uint8_t *src;
    const uint8_t *dst;

    if (!funchook_jump32_avail(addr, entry->trampoline)) {
        funchook_log(funchook, "  could not jump function %p to trampoline %p\n", addr, entry->trampoline);
        return 0;
    }
    src = entry->trampoline + disp[0].src_addr_offset;
    dst = disp[0].dst_addr;
    if (!funchook_within_32bit_relative(src, dst)) {
        funchook_log(funchook, "  could not jump trampoline %p to function %p\n",
                     src, dst);
        return 0;
    }
    src = entry->trampoline + disp[1].src_addr_offset;
    dst = disp[1].dst_addr;
    if (dst != 0 && !funchook_within_32bit_relative(src, dst)) {
        funchook_log(funchook, "  could not make 32-bit relative address from %p to %p\n",
                     src, dst);
        return 0;
    }
    return 1;
}

#endif

funchook_t *funchook_create(void)
{
    funchook_t *funchook = NULL;

    funchook_log(funchook, "Enter funchook_create()\n");
    funchook = funchook_create_internal();
    funchook_log_end(funchook, "Leave funchook_create() => %p\n", funchook);
    return funchook;
}

int funchook_prepare(funchook_t *funchook, void **target_func, void *hook_func)
{
    int rv;
    void *orig_func;

    funchook_log(funchook, "Enter funchook_prepare(%p, %p, %p)\n", funchook, target_func, hook_func);
    orig_func = *target_func;
    rv = funchook_prepare_internal(funchook, target_func, hook_func);
    funchook_log_end(funchook, "Leave funchook_prepare(..., [%p->%p],...) => %d\n", orig_func, *target_func, rv);
    return rv;
}

int funchook_install(funchook_t *funchook, int flags)
{
    int rv;

    funchook_log(funchook, "Enter funchook_install(%p, 0x%x)\n", funchook, flags);
    rv = funchook_install_internal(funchook, flags);
    funchook_log_end(funchook, "Leave funchook_install() => %d\n", rv);
    return rv;
}

int funchook_uninstall(funchook_t *funchook, int flags)
{
    int rv;

    funchook_log(funchook, "Enter funchook_uninstall(%p, 0x%x)\n", funchook, flags);
    rv = funchook_uninstall_internal(funchook, flags);
    funchook_log_end(NULL, "Leave funchook_uninstall() => %d\n", rv);
    return rv;
}

int funchook_destroy(funchook_t *funchook)
{
    int rv;

    funchook_log(funchook, "Enter funchook_destroy(%p)\n", funchook);
    rv = funchook_destroy_internal(funchook);
    funchook_log_end(rv == 0 ? NULL : funchook, "Leave funchook_destroy() => %d\n", rv);
    return rv;
}

const char *funchook_error_message(const funchook_t *funchook)
{
    return funchook->error_message;
}

int funchook_set_debug_file(const char *name)
{
    if (name != NULL) {
        strncpy(funchook_debug_file, name, sizeof(funchook_debug_file) - 1);
        funchook_debug_file[sizeof(funchook_debug_file) - 1] = '\0';
    } else {
        funchook_debug_file[0] = '\0';
    }
    return 0;
}

void funchook_log(funchook_t *funchook, const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    funchook_logv(funchook, 0, fmt, ap);
    va_end(ap);
}

void funchook_set_error_message(funchook_t *funchook, const char *fmt, ...)
{
    va_list ap;

    va_start(ap, fmt);
    vsnprintf(funchook->error_message, FUNCHOOK_MAX_ERROR_MESSAGE_LEN, fmt, ap);
    va_end(ap);
    va_start(ap, fmt);
    funchook_logv(funchook, 1, fmt, ap);
    va_end(ap);
}

static void funchook_logv(funchook_t *funchook, int set_error, const char *fmt, va_list ap)
{
    FILE *fp;

    if (*funchook_debug_file == '\0') {
        return;
    }
    if (funchook == NULL) {
        fp = fopen(funchook_debug_file, "a");
    } else if (funchook->fp == NULL) {
        funchook->fp = fopen(funchook_debug_file, "a");
        fp = funchook->fp;
    } else {
        fp = funchook->fp;
    }
    if (fp == NULL) {
        return;
    }
    if (set_error) {
        fputs("  ", fp);
    }
    vfprintf(fp, fmt, ap);
    if (set_error) {
        fputc('\n', fp);
    }
    if (funchook == NULL) {
        fclose(fp);
    } else {
        fflush(fp);
    }
}

static void funchook_log_end(funchook_t *funchook, const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    funchook_logv(funchook, 0, fmt, ap);
    va_end(ap);
    if (funchook != NULL && funchook->fp != NULL) {
        fclose(funchook->fp);
        funchook->fp = NULL;
    }
}

static funchook_t *funchook_create_internal(void)
{
    funchook_t *funchook = funchook_alloc();
    if (funchook == NULL) {
        return NULL;
    }
    if (num_entries_in_page == 0) {
        num_entries_in_page = (page_size - offsetof(funchook_page_t, entries)) / sizeof(funchook_entry_t);
        funchook_log(funchook,
#ifdef WIN32
                     "  allocation_unit=%"SIZE_T_FMT"u\n"
#endif
                     "  page_size=%"SIZE_T_FMT"u\n"
                     "  num_entries_in_page=%"SIZE_T_FMT"u\n",
#ifdef WIN32
                     allocation_unit,
#endif
                     page_size, num_entries_in_page);
    }
    return funchook;
}

static int funchook_prepare_internal(funchook_t *funchook, void **target_func, void *hook_func)
{
    void *func = *target_func;
    rip_displacement_t disp[2] = {{0,},{0,}};
    funchook_page_t *page = NULL;
    funchook_entry_t *entry;
    uint8_t *src_addr;
    uint32_t *offset_addr;
    int rv;

    if (funchook->installed) {
        funchook_set_error_message(funchook, "Could not modify already-installed funchook handle.");
        return FUNCHOOK_ERROR_ALREADY_INSTALLED;
    }
    func = funchook_resolve_func(funchook, func);
    
    rv = get_page(funchook, &page, func, disp);
    if (rv != 0) {
        funchook_log(funchook, "  failed to get page\n");
        return rv;
    }
    entry = &page->entries[page->used];
    /* fill members */
    entry->target_func = func;
    entry->hook_func = hook_func;
    
#ifdef CPU_PPC64
    entry->_jump32ToNew = funchook_jump32_avail(func, entry->proxy_code) ? 1 : 0 ;
    entry->_beginWithToc = BeginWith_TOC_Code(func);
    rv = funchook_make_trampoline(funchook, disp, func, (uint8_t*)entry);
#else 
    rv = funchook_make_trampoline(funchook, disp, func, entry->trampoline);
#endif

    if (rv != 0) {
        funchook_log(funchook, "  failed to make trampoline\n");
        return rv;
    }

    memcpy(entry->old_code, func, JUMP32_SIZE);
#ifdef CPU_X86_64
    if (funchook_jump32_avail(func, hook_func)) {
        funchook_write_jump32(funchook, func, hook_func, entry->new_code);
        entry->transit[0] = 0;
    } else {
        funchook_write_jump32(funchook, func, entry->transit, entry->new_code);
        funchook_write_jump64(funchook, entry->transit, hook_func);
    }
#elif defined  CPU_PPC64
    if (entry->_jump32ToNew) {
        funchook_write_jump32(funchook, func, hook_func, (uint8_t*)entry);
    } else {
        funchook_write_jump64(funchook, func, hook_func, (uint8_t*)entry);
    }
#else
    funchook_write_jump32(funchook, func, hook_func, entry->new_code);
#endif

    

#ifndef CPU_PPC64
    /* fix rip-relative offsets */
    src_addr = entry->trampoline + disp[0].src_addr_offset;
    offset_addr = (uint32_t*)(entry->trampoline + disp[0].pos_offset);
    *offset_addr = (uint32_t)(disp[0].dst_addr - src_addr);
    if (disp[1].dst_addr != 0) {
        src_addr = entry->trampoline + disp[1].src_addr_offset;
        offset_addr = (uint32_t*)(entry->trampoline + disp[1].pos_offset);
        *offset_addr = (uint32_t)(disp[1].dst_addr - src_addr);
    }
    funchook_log_trampoline(funchook, entry->trampoline);
#endif

    page->used++;
    *target_func = (void*)entry->trampoline;
    return 0;
}

static int funchook_install_internal(funchook_t *funchook, int flags)
{
    funchook_page_t *page;

    if (funchook->installed) {
        return FUNCHOOK_ERROR_ALREADY_INSTALLED;
    }

    for (page = funchook->page_list; page != NULL; page = page->next) {
        int rv = funchook_page_protect(funchook, page);
        int i;

        if (rv != 0) {
            return rv;
        }

        for (i = 0; i < page->used; i++) {
            funchook_entry_t *entry = &page->entries[i];
            mem_state_t mstate;
            int rv = funchook_unprotect_begin(funchook, &mstate, entry->target_func, JUMP32_SIZE);

            if (rv != 0) {
                return rv;
            }
            #ifdef CPU_PPC64
            memcpy(entry->target_func, entry->new_code,  (entry->_jump32ToNew ? JUMP32_SIZE : JUMP64_SIZE) + (entry->_beginWithToc ? 8 : 0));
            #else
            memcpy(entry->target_func, entry->new_code,  JUMP32_SIZE);
            #endif 
            rv = funchook_unprotect_end(funchook, &mstate);
            if (rv != 0) {
                return rv;
            }
        }
    }
    funchook->installed = 1;
    return 0;
}

static int funchook_uninstall_internal(funchook_t *funchook, int flags)
{
    funchook_page_t *page;

    if (!funchook->installed) {
        return FUNCHOOK_ERROR_NOT_INSTALLED;
    }

    for (page = funchook->page_list; page != NULL; page = page->next) {
        int i;

        for (i = 0; i < page->used; i++) {
            funchook_entry_t *entry = &page->entries[i];
            mem_state_t mstate;
            int rv = funchook_unprotect_begin(funchook, &mstate, entry->target_func, JUMP32_SIZE);

            if (rv != 0) {
                return rv;
            }
            memcpy(entry->target_func, entry->old_code, JUMP32_SIZE);
            rv = funchook_unprotect_end(funchook, &mstate);
            if (rv != 0) {
                return rv;
            }
        }
        funchook_page_unprotect(funchook, page);
    }
    funchook->installed = 0;
    return 0;
}

static int funchook_destroy_internal(funchook_t *funchook)
{
    funchook_page_t *page, *page_next;

    if (funchook == NULL) {
       return -1;
    }
    if (funchook->installed) {
        return FUNCHOOK_ERROR_ALREADY_INSTALLED;
    }
    for (page = funchook->page_list; page != NULL; page = page_next) {
        page_next = page->next;
        funchook_page_free(funchook, page);
    }
    if (funchook->fp != NULL) {
        fclose(funchook->fp);
    }
    funchook_free(funchook);
    return 0;
}

static int get_page(funchook_t *funchook, funchook_page_t **page_out, uint8_t *addr, rip_displacement_t *disp)
{
    funchook_page_t *page;
    int rv;

    for (page = funchook->page_list; page != NULL; page = page->next) {
        if (page->used < num_entries_in_page && funchook_page_avail(funchook, page, page->used, addr, disp)) {
            /* Reuse allocated page. */
            *page_out = page;
            return 0;
        }
    }
    rv = funchook_page_alloc(funchook, &page, addr, disp);
    if (rv != 0) {
        return rv;
    }
    page->used = 0;
    page->next = funchook->page_list;
    funchook->page_list = page;
    *page_out = page;
    return 0;
}
