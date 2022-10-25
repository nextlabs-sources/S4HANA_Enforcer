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
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <distorm.h>
#include <mnemonics.h>
#ifdef WIN32
#include <windows.h>
#endif
#include "funchook_internal.h"

/* RIP-relative address information */
typedef struct {
    uint8_t *addr; /* absolute address */
    intptr_t raddr; /* relative address */
    int offset;
    int size;
} rip_relative_t;

typedef struct {
    funchook_t *funchook;
    rip_displacement_t *rip_disp;
    const uint8_t *src;
    const uint8_t *dst_base;
    uint8_t *dst;
} make_trampoline_context_t;

#define NOP_INSTRUCTION 0x60000000
#define PPC_INSTRUCT_LEN 4

#if defined(__i386)
static int handle_x86_get_pc_thunk(make_trampoline_context_t *ctx, const _DInst *di);
static int handle_x86_get_pc_by_call_and_pop(make_trampoline_context_t *ctx, const _DInst *di);
#else
#define handle_x86_get_pc_thunk(ctx, di) (0)
#define handle_x86_get_pc_by_call_and_pop(ctx, di) (0)
#endif

static void log_instruction(funchook_t *funchook, const _CodeInfo *ci, const _DInst *dis);
static void get_rip_relative(const make_trampoline_context_t *ctx, rip_relative_t *rel_disp, rip_relative_t *rel_imm, const _DInst *di);
static int handle_rip_relative(make_trampoline_context_t *ctx, const rip_relative_t *rel, const _DInst *di);

int ConstructIns_B(int src, int dst, int aa, int lk)
{
	return ((lk & 0x00000001) | (aa & 0x00000002) | ((dst - src)&0x03ffffff) | (18<<26) ) ;
}

int ConstructIns_STD(int rs, int ra, int ds)
{
	return ((ds&0x0000ffff) | (ra << 16) | (rs << 21) | (62 << 26));
}

int ConstructIns_LD(int rt, int ra, int ds)
{
	return ((ds & 0x0000ffff) | (ra << 16) | (rt << 21) | (58 << 26));
}

int ConstructIns_ADDI(int rt, int ra, int v)
{
	return ((v & 0x0000ffff) | (ra << 16) | (rt << 21) | (14 << 26));
}

int ConstructIns_ADDIS(int rt, int ra, int v)
{
	return ((v & 0x0000ffff) | (ra << 16) | (rt << 21) | (15 << 26));
}

int ConstructIns_MTSPR(int rs, int spr)
{
	//The order of the two 5-bit halves of the SPR number is reversed
	return  ((467 << 1) | (((spr&0x000003E0) << 11) | ((spr&0x0000001f) << 16)) | (rs << 21) | (31 << 26));
}

int ConstructIns_BCCTR(int bo, int bi, int bh, int lk)
{
	return  ((lk) | (528 << 1) | (bh<<11) | (bi << 16) | (bo<<21) | (19 << 26));
}

int ConstructIns_BCTR()
{
	return ConstructIns_BCCTR(20, 0, 0, 0);
}

int ConstructIns_ORI(int ra, int rs, uint16_t v)
{
	return ((v) | (ra<<16) | (rs<<21) | (24<<26) );
}

int ConstructIns_ORIS(int ra, int rs, uint16_t v)
{
	return ((v) | (ra << 16) | (rs << 21) | (25 << 26));
}

int ConstructIns_rldicr(int ra, int rs, int sh, int me)
{
	int rc = 0;
 	return ( rc | (((sh&0x20)>>5)<<1) | (1<<2) | ((me&0x20) | ((me&0x1f)<<6) )| (((sh&0x1f))<<11) |(ra << 16) | (rs << 21) | (30 << 26));
}

//is table of content code, this code didn't execute, so must be skip when hook
int BeginWith_TOC_Code(const uint8_t* p)
{
	uint32_t ins1 = *(uint32_t*)p;
	uint32_t ins2 = *((uint32_t*)p + 1);

	return ((ins1 & 0xFFE00000) == (((uint32_t)2 << 21) | ((uint32_t)15 << 26))) &&
		((ins2 & 0xFFE00000) == (((uint32_t)2 << 21) | ((uint32_t)14 << 26)));
}


int funchook_write_jump32(funchook_t *funchook, const uint8_t *src, const uint8_t *dst, uint8_t *inEntry)
{
    funchook_entry_t *entry = (funchook_entry_t *)inEntry;

    int* pOut = (int*)entry->new_code;
    int nInsIndex = 0;
    const uint8_t* pSrc = src;
    if (entry->_beginWithToc)
    {
        //insert two nop instruciton. because the instruction will be execute begin with 3th instruction.
        pOut[nInsIndex++] = NOP_INSTRUCTION;
        pSrc+=4;
        pOut[nInsIndex++] = NOP_INSTRUCTION;
        pSrc+=4;
    }
    
    pOut[nInsIndex++]= ConstructIns_B(pSrc, entry->proxy_code, 0, 0); //jump to proxy code.
 
    funchook_log(funchook, "  Write jump32 0x%0"SIZE_T_WIDTH SIZE_T_FMT"x -> 0x%0"SIZE_T_WIDTH SIZE_T_FMT"x, proxy_code=%p\n",
                 (size_t)src, (size_t)dst, entry->proxy_code);
    return 0;
}



int funchook_write_jump64(funchook_t *funchook, const uint8_t *src, const uint8_t *dst, uint8_t *inEntry)
{
    funchook_entry_t *entry = (funchook_entry_t *)inEntry;

    int* pOut = (int*)entry->new_code;
    int nInsIndex = 0;

    if (entry->_beginWithToc)
    {
        //insert two nop instruciton. because the instruction will be execute begin with 3th instruction.
        pOut[nInsIndex++] = NOP_INSTRUCTION;
        pOut[nInsIndex++] = NOP_INSTRUCTION;
    }

    //jump to proxy_code
    uint32_t* pProxyCode = (uint32_t*)entry->hook_func;
    pOut[nInsIndex++] = ConstructIns_ADDIS(12, 0, ((int64_t)pProxyCode&0xffff000000000000)>>48);
    pOut[nInsIndex++] = ConstructIns_ORI(12, 12, ((int64_t)pProxyCode&0x0000ffff00000000)>>32);
    pOut[nInsIndex++] = ConstructIns_rldicr(12, 12, 32, 31);
    pOut[nInsIndex++] = ConstructIns_ORIS(12, 12, ((int64_t)pProxyCode&0x00000000ffff0000)>>16);
    pOut[nInsIndex++] = ConstructIns_ORI(12, 12, ((int64_t)pProxyCode&0x000000000000ffff)>>0);

    //move r12 to ctr
    pOut[nInsIndex++] = ConstructIns_MTSPR(12, 9);
 
    //branch to ctr
    pOut[nInsIndex++] = ConstructIns_BCTR();

    funchook_log(funchook, "  Write jump64 0x%0"SIZE_T_WIDTH SIZE_T_FMT"x -> 0x%0"SIZE_T_WIDTH SIZE_T_FMT"x\n",
                 (size_t)src, (size_t)dst);
    return 0;
}

int funchook_within_32bit_relative(const uint8_t *src, const uint8_t *dst)
{
    const static int32_t INT26_MIN = 0xFE000000;
    const static int32_t INT26_MAX = 0x01000000;
    int64_t diff = (int64_t)(dst - src);
    return ((INT26_MIN<=diff) && (diff<=INT26_MAX));
}

int funchook_jump32_avail(const uint8_t *src, const uint8_t *dst)
{
    int bJump32Avail = funchook_within_32bit_relative(src, dst);
    printf("funchook_jump32_avail, src=%p, dst=%p, bJump32Avail=%d\n", src, dst, bJump32Avail);
    return bJump32Avail;
}


int funchook_make_trampoline(funchook_t *funchook, rip_displacement_t *disp, const uint8_t *func, uint8_t *inEntry)
{
    funchook_entry_t *entry = (funchook_entry_t *)inEntry;
    uint8_t* trampoline = entry->trampoline;
    make_trampoline_context_t ctx;
    _DInst dis[MAX_INSN_CHECK_SIZE];
    unsigned int di_cnt = 0;
    _CodeInfo ci;
    int nInsIndex = 0;
    int rv;
    unsigned int i;
    int* pTrampoline = (int*)trampoline;
    for(i=0; i<TRAMPOLINE_SIZE/4; i++){
        pTrampoline[i] = NOP_INSTRUCTION;
    }

    ctx.funchook = funchook;
    ctx.rip_disp = disp;
    ctx.src = func;
    ctx.dst_base = ctx.dst = trampoline;

    ci.codeOffset = (_OffsetType)(size_t)func;
    ci.code = func;
    ci.codeLen = MAX_INSN_CHECK_SIZE;
#ifdef CPU_X86_64
    ci.dt = Decode64Bits;
#else
    ci.dt = Decode32Bits;
#endif
    ci.features = DF_STOP_ON_RET;

    const size_t nCopyOldCode =  (entry->_jump32ToNew ? JUMP32_SIZE : JUMP64_SIZE ) + (entry->_beginWithToc ? 8 /*added two toc instruction*/: 0);
    
    nInsIndex = 0;
    //construct ctr for jump to the old code     
  //  pTrampoline[nInsIndex++] = ConstructIns_STD(3, 1, -8); // std r3, -8(r1), save r3 to r1-8,  //backup r3

    uint8_t* pOriginalFunDst = func + nCopyOldCode;

    pTrampoline[nInsIndex++] = ConstructIns_ADDIS(12, 0, ((int64_t)pOriginalFunDst&0xffff000000000000)>>48);
    pTrampoline[nInsIndex++] = ConstructIns_ORI(12, 12, ((int64_t)pOriginalFunDst&0x0000ffff00000000)>>32);
    pTrampoline[nInsIndex++] = ConstructIns_rldicr(12, 12, 32, 31);
    pTrampoline[nInsIndex++] = ConstructIns_ORIS(12, 12, ((int64_t)pOriginalFunDst&0x00000000ffff0000)>>16);
    pTrampoline[nInsIndex++] = ConstructIns_ORI(12, 12, ((int64_t)pOriginalFunDst&0x000000000000ffff)>>0);

    pTrampoline[nInsIndex++] = ConstructIns_MTSPR(12, 9); ////move r3 to ctr
   
    //set r12 to the enter of fun
    pTrampoline[nInsIndex++] = ConstructIns_ADDI(12, 12, 0-nCopyOldCode); 
  
    //copy old instrcution 
     memcpy(pTrampoline+nInsIndex , ctx.src, nCopyOldCode);
     ctx.dst += nInsIndex*4 + nCopyOldCode;
     ctx.src += nInsIndex*4 + nCopyOldCode;
     nInsIndex += nCopyOldCode/4;

     //jump to old code
    pTrampoline[nInsIndex++] = ConstructIns_BCTR();

    //make proxy code to do preparing and jump to hook_function
    {
        memset(entry->proxy_code, 0, sizeof(entry->proxy_code));
        uint32_t* pProxyCode = (uint32_t*)entry->proxy_code;
        uint32_t* pHookFun = (uint32_t*)entry->hook_func;
        int nProxyInsIndex = 0;
        //fill r12 with hook fun address, r12 will be used to calculate TOC
        pProxyCode[nProxyInsIndex++] = ConstructIns_ADDIS(12, 0, ((int64_t)pHookFun&0xffff000000000000)>>48);
        pProxyCode[nProxyInsIndex++] = ConstructIns_ORI(12, 12, ((int64_t)pHookFun&0x0000ffff00000000)>>32);
        pProxyCode[nProxyInsIndex++] = ConstructIns_rldicr(12, 12, 32, 31);
        pProxyCode[nProxyInsIndex++] = ConstructIns_ORIS(12, 12, ((int64_t)pHookFun&0x00000000ffff0000)>>16);
        pProxyCode[nProxyInsIndex++] = ConstructIns_ORI(12, 12, ((int64_t)pHookFun&0x000000000000ffff)>>0);

        //move r3 to ctr
        pProxyCode[nProxyInsIndex++] = ConstructIns_MTSPR(12, 9);
  
       //branch to ctr
        pProxyCode[nProxyInsIndex++] = ConstructIns_BCTR();
    }

    //modify instruction on hook_func, reset r2 to target function
     if (entry->_beginWithToc)
     { 
        const int32_t* pHookFun = entry->hook_func;
        int nInsIndex = 10;
        const int32_t nopIns[7] = {NOP_INSTRUCTION,NOP_INSTRUCTION,NOP_INSTRUCTION,NOP_INSTRUCTION,NOP_INSTRUCTION,NOP_INSTRUCTION,NOP_INSTRUCTION}; 
        int32_t* pInsSpace = 0;
        while (pHookFun[nInsIndex] != 0x4e800020 /*blr*/)
        {
            if(memcmp(pHookFun+nInsIndex, nopIns, sizeof(nopIns))==0 ){
                pInsSpace = pHookFun+nInsIndex;
                break;
            }
            nInsIndex++;
        }

        if(pInsSpace){
             mem_state_t mstate;
             int rv = funchook_unprotect_begin(funchook, &mstate, entry->hook_func, JUMP32_SIZE);
              if (rv != 0) {
                return rv;
              }
            int32_t* pTargetFun = entry->target_func;
            pInsSpace[0] = ConstructIns_ADDIS(12, 0, ((int64_t)pTargetFun&0xffff000000000000)>>48);
            pInsSpace[1] = ConstructIns_ORI(12, 12, ((int64_t)pTargetFun&0x0000ffff00000000)>>32);
            pInsSpace[2] = ConstructIns_rldicr(12, 12, 32, 31);
            pInsSpace[3] = ConstructIns_ORIS(12, 12, ((int64_t)pTargetFun&0x00000000ffff0000)>>16);
            pInsSpace[4] = ConstructIns_ORI(12, 12, ((int64_t)pTargetFun&0x000000000000ffff)>>0);
            pInsSpace[5] = pTargetFun[0];
            pInsSpace[6] = pTargetFun[1];

            rv = funchook_unprotect_end(funchook, &mstate);
            if (rv != 0) {
                return rv;
            }
        }
        
    }


    return 0;
}

void funchook_log_trampoline(funchook_t *funchook, const uint8_t *trampoline)
{
    _DInst dis[TRAMPOLINE_SIZE];
    unsigned int di_cnt = 0;
    _CodeInfo ci;
    _DecodeResult decres;
    unsigned int i;

    if (*funchook_debug_file == '\0') {
        return;
    }

    funchook_log(funchook, "  Trampoline Instructions:\n");
    ci.codeOffset = (_OffsetType)(size_t)trampoline;
    ci.code = trampoline;
    ci.codeLen = TRAMPOLINE_SIZE;
#ifdef CPU_X86_64
    ci.dt = Decode64Bits;
#else
    ci.dt = Decode32Bits;
#endif
    ci.features = DF_NONE;
    decres = distorm_decompose64(&ci, dis, TRAMPOLINE_SIZE, &di_cnt);
    if (decres != DECRES_SUCCESS) {
        funchook_log(funchook, "  Failed to decode trampoline\n    ");
        for (i = 0; i < TRAMPOLINE_SIZE; i++) {
            funchook_log(funchook, " %02x", trampoline[i]);
        }
        funchook_log(funchook, "\n");
        return;
    }
    while (di_cnt > 0 && dis[di_cnt - 1].opcode == I_NOP) {
        di_cnt--;
    }
    for (i = 0; i < di_cnt; i++) {
        _DecodedInst dec;
        distorm_format64(&ci, &dis[i], &dec);
        funchook_log(funchook, "    %0"SIZE_T_WIDTH SIZE_T_FMT"x (%02d) %-24s %s%s%s\n",
                     (size_t)dec.offset, dec.size, (char*)dec.instructionHex.p,
                     (char*)dec.mnemonic.p, dec.operands.length != 0 ? " " : "", (char*)dec.operands.p);
    }
}

#ifndef handle_x86_get_pc_thunk
/* special cases to handle "call __x86.get_pc_thunk.??"
 * If the target instructions are "movl (%esp), %???; ret",
 * use "movl di->addr + 5, %???" instead.
 */
static int handle_x86_get_pc_thunk(make_trampoline_context_t *ctx, const _DInst *di)
{
    uint32_t eip = 0;
    const char *reg_name = NULL;

    if (*ctx->src == 0xe8) {
        uint32_t first_4_bytes = *(uint32_t*)(size_t)INSTRUCTION_GET_TARGET(di);

        eip = (uint32_t)(di->addr + 5);
        switch (first_4_bytes) {
        case 0xc324048b: /* 8b 04 24 c3: movl (%esp), %eax; ret */
            reg_name = "ax";
            *ctx->dst = 0xb8; /*         movl di->addr + 5, %eax */
            *(uint32_t*)(ctx->dst + 1) = eip;
            goto fixed;
        case 0xc3241c8b: /* 8b 1c 24 c3: movl (%esp), %ebx; ret */
            reg_name = "bx";
            *ctx->dst = 0xbb; /*         movl di->addr + 5, %ebx */
            *(uint32_t*)(ctx->dst + 1) = eip;
            goto fixed;
        case 0xc3240c8b: /* 8b 0c 24 c3: movl (%esp), %ecx; ret */
            reg_name = "cx";
            *ctx->dst = 0xb9; /*         movl di->addr + 5, %ecx */
            *(uint32_t*)(ctx->dst + 1) = eip;
            goto fixed;
        case 0xc324148b: /* 8b 14 24 c3: movl (%esp), %edx; ret */
            reg_name = "dx";
            *ctx->dst = 0xba; /*         movl di->addr + 5, %edx */
            *(uint32_t*)(ctx->dst + 1) = eip;
            goto fixed;
        case 0xc324348b: /* 8b 34 24 c3: movl (%esp), %esi; ret */
            reg_name = "si";
            *ctx->dst = 0xbe; /*         movl di->addr + 5, %esi */
            *(uint32_t*)(ctx->dst + 1) = eip;
            goto fixed;
        case 0xc3243c8b: /* 8b 3c 24 c3: movl (%esp), %edi; ret */
            reg_name = "di";
            *ctx->dst = 0xbf; /*         movl di->addr + 5, %edi */
            *(uint32_t*)(ctx->dst + 1) = eip;
            goto fixed;
        case 0xc3242c8b: /* 8b 2c 24 c3: movl (%esp), %ebp; ret */
            reg_name = "bp";
            *ctx->dst = 0xbd; /*         movl di->addr + 5, %ebp */
            *(uint32_t*)(ctx->dst + 1) = eip;
            goto fixed;
        }
    }
    return 0;

fixed:
    funchook_log(ctx->funchook, "      use 'MOV E%c%c, 0x%x' instead of 'CALL __x86.get_pc_thunk.%s'\n",
                 reg_name[0] + 'A' - 'a',
                 reg_name[1] + 'A' - 'a',
                 eip, reg_name);
    ctx->dst += 5;
    ctx->src += 5;
    return 1;
}
#endif

#ifndef handle_x86_get_pc_by_call_and_pop
static int handle_x86_get_pc_by_call_and_pop(make_trampoline_context_t *ctx, const _DInst *di)
{
    uint32_t eip = 0;
    const char *reg_name = NULL;

    if (*ctx->src == 0xe8 && *(uint32_t*)(ctx->src + 1) == 0) {
        eip = (uint32_t)(di->addr + 5);
        switch (*(ctx->src + 5)) {
        case 0x58: /* pop %eax */
            reg_name = "EAX";
            *ctx->dst = 0xb8; /* movl di->addr + 5, %eax */
            *(uint32_t*)(ctx->dst + 1) = eip;
            goto fixed;
        case 0x5b: /* pop %ebx */
            reg_name = "EBX";
            *ctx->dst = 0xbb; /* movl di->addr + 5, %ebx */
            *(uint32_t*)(ctx->dst + 1) = eip;
            goto fixed;
        case 0x59: /* pop %ecx */
            reg_name = "ECX";
            *ctx->dst = 0xb9; /* movl di->addr + 5, %ecx */
            *(uint32_t*)(ctx->dst + 1) = eip;
            goto fixed;
        case 0x5a: /* pop %edx */
            reg_name = "EDX";
            *ctx->dst = 0xba; /* movl di->addr + 5, %edx */
            *(uint32_t*)(ctx->dst + 1) = eip;
            goto fixed;
        case 0x5e: /* pop %esi */
            reg_name = "ESI";
            *ctx->dst = 0xbe; /* movl di->addr + 5, %esi */
            *(uint32_t*)(ctx->dst + 1) = eip;
            goto fixed;
        case 0x5f: /* pop %edi */
            reg_name = "EDI";
            *ctx->dst = 0xbf; /* movl di->addr + 5, %edi */
            *(uint32_t*)(ctx->dst + 1) = eip;
            goto fixed;
        case 0x5d: /* pop %ebp */
            reg_name = "EBP";
            *ctx->dst = 0xbd; /* movl di->addr + 5, %ebp */
            *(uint32_t*)(ctx->dst + 1) = eip;
            goto fixed;
        }
    }
    return 0;

fixed:
    funchook_log(ctx->funchook, "      use 'MOV %s, 0x%x' instead of 'CALL 0x%x; POP %s'\n",
                 reg_name, eip, eip, reg_name);
    ctx->dst += 5;
    ctx->src += 6;
    return 1;

}
#endif

static void log_instruction(funchook_t *funchook, const _CodeInfo *ci, const _DInst *dis)
{
    _DecodedInst dec;
    distorm_format64(ci, dis, &dec);
    funchook_log(funchook, "    %0"SIZE_T_WIDTH SIZE_T_FMT"x (%02d) %-24s %s%s%s\n",
                 (size_t)dec.offset, dec.size, (char*)dec.instructionHex.p,
                 (char*)dec.mnemonic.p, dec.operands.length != 0 ? " " : "", (char*)dec.operands.p);
}

static void get_rip_relative(const make_trampoline_context_t *ctx, rip_relative_t *rel_disp, rip_relative_t *rel_imm, const _DInst *di)
{
    int opsiz = 0;
    int disp_offset = 0;
    int imm_offset = 0;
    int i;

    memset(rel_disp, 0, sizeof(rip_relative_t));
    memset(rel_imm, 0, sizeof(rip_relative_t));

    /*
     * Estimate total operand size and RIP-relative address offsets.
     */
    for (i = 0; i < OPERANDS_NO && di->ops[i].type != O_NONE; i++) {
        const _Operand *op = &di->ops[i];
        switch (op->type) {
        case O_IMM:
            opsiz += op->size / 8;
            break;
        case O_PC:
            rel_imm->addr = (uint8_t*)(size_t)(di->addr + di->size + di->imm.addr);
            rel_imm->raddr = (intptr_t)di->imm.addr;
            rel_imm->size = op->size;
            imm_offset = opsiz;
            opsiz += op->size / 8;
            break;
        case O_SMEM:
            if (di->dispSize != 0 && op->index == R_RIP) {
                rel_disp->addr = (uint8_t*)(size_t)(di->addr + di->size + di->disp);
                rel_disp->raddr = (intptr_t)di->disp;
                rel_disp->size = di->dispSize;
                disp_offset = opsiz;
            }
            opsiz += di->dispSize / 8;
            break;
        case O_MEM:
        case O_DISP:
            opsiz += di->dispSize / 8;
            break;
        }
    }
    switch (di->opcode) {
    /* CMPSD */
    case I_CMPEQSD:
    case I_CMPLTSD:
    case I_CMPLESD:
    case I_CMPUNORDSD:
    case I_CMPNEQSD:
    case I_CMPNLTSD:
    case I_CMPNLESD:
    case I_CMPORDSD:
    case I_VCMPEQSD:
    case I_VCMPLTSD:
    case I_VCMPLESD:
    case I_VCMPUNORDSD:
    case I_VCMPNEQSD:
    case I_VCMPNLTSD:
    case I_VCMPNLESD:
    case I_VCMPORDSD:
    case I_VCMPEQ_UQSD:
    case I_VCMPNGESD:
    case I_VCMPNGTSD:
    case I_VCMPFALSESD:
    case I_VCMPNEQ_OQSD:
    case I_VCMPGESD:
    case I_VCMPGTSD:
    case I_VCMPTRUESD:
    case I_VCMPEQ_OSSD:
    case I_VCMPLT_OQSD:
    case I_VCMPLE_OQSD:
    case I_VCMPUNORD_SSD:
    case I_VCMPNEQ_USSD:
    case I_VCMPNLT_UQSD:
    case I_VCMPNLE_UQSD:
    case I_VCMPORD_SSD:
    case I_VCMPEQ_USSD:
    case I_VCMPNGE_UQSD:
    case I_VCMPNGT_UQSD:
    case I_VCMPFALSE_OSSD:
    case I_VCMPNEQ_OSSD:
    case I_VCMPGE_OQSD:
    case I_VCMPGT_OQSD:
    /* CMPSS */
    case I_CMPEQSS:
    case I_CMPLTSS:
    case I_CMPLESS:
    case I_CMPUNORDSS:
    case I_CMPNEQSS:
    case I_CMPNLTSS:
    case I_CMPNLESS:
    case I_CMPORDSS:
    case I_VCMPEQSS:
    case I_VCMPLTSS:
    case I_VCMPLESS:
    case I_VCMPUNORDSS:
    case I_VCMPNEQSS:
    case I_VCMPNLTSS:
    case I_VCMPNLESS:
    case I_VCMPORDSS:
    case I_VCMPEQ_UQSS:
    case I_VCMPNGESS:
    case I_VCMPNGTSS:
    case I_VCMPFALSESS:
    case I_VCMPNEQ_OQSS:
    case I_VCMPGESS:
    case I_VCMPGTSS:
    case I_VCMPTRUESS:
    case I_VCMPEQ_OSSS:
    case I_VCMPLT_OQSS:
    case I_VCMPLE_OQSS:
    case I_VCMPUNORD_SSS:
    case I_VCMPNEQ_USSS:
    case I_VCMPNLT_UQSS:
    case I_VCMPNLE_UQSS:
    case I_VCMPORD_SSS:
    case I_VCMPEQ_USSS:
    case I_VCMPNGE_UQSS:
    case I_VCMPNGT_UQSS:
    case I_VCMPFALSE_OSSS:
    case I_VCMPNEQ_OSSS:
    case I_VCMPGE_OQSS:
    case I_VCMPGT_OQSS:
    /* CMPPD */
    case I_CMPEQPD:
    case I_CMPLTPD:
    case I_CMPLEPD:
    case I_CMPUNORDPD:
    case I_CMPNEQPD:
    case I_CMPNLTPD:
    case I_CMPNLEPD:
    case I_CMPORDPD:
    case I_VCMPEQPD:
    case I_VCMPLTPD:
    case I_VCMPLEPD:
    case I_VCMPUNORDPD:
    case I_VCMPNEQPD:
    case I_VCMPNLTPD:
    case I_VCMPNLEPD:
    case I_VCMPORDPD:
    case I_VCMPEQ_UQPD:
    case I_VCMPNGEPD:
    case I_VCMPNGTPD:
    case I_VCMPFALSEPD:
    case I_VCMPNEQ_OQPD:
    case I_VCMPGEPD:
    case I_VCMPGTPD:
    case I_VCMPTRUEPD:
    case I_VCMPEQ_OSPD:
    case I_VCMPLT_OQPD:
    case I_VCMPLE_OQPD:
    case I_VCMPUNORD_SPD:
    case I_VCMPNEQ_USPD:
    case I_VCMPNLT_UQPD:
    case I_VCMPNLE_UQPD:
    case I_VCMPORD_SPD:
    case I_VCMPEQ_USPD:
    case I_VCMPNGE_UQPD:
    case I_VCMPNGT_UQPD:
    case I_VCMPFALSE_OSPD:
    case I_VCMPNEQ_OSPD:
    case I_VCMPGE_OQPD:
    case I_VCMPGT_OQPD:
    case I_VCMPTRUE_USPD:
    /* CMPPS */
    case I_CMPEQPS:
    case I_CMPLTPS:
    case I_CMPLEPS:
    case I_CMPUNORDPS:
    case I_CMPNEQPS:
    case I_CMPNLTPS:
    case I_CMPNLEPS:
    case I_CMPORDPS:
    case I_VCMPEQPS:
    case I_VCMPLTPS:
    case I_VCMPLEPS:
    case I_VCMPUNORDPS:
    case I_VCMPNEQPS:
    case I_VCMPNLTPS:
    case I_VCMPNLEPS:
    case I_VCMPORDPS:
    case I_VCMPEQ_UQPS:
    case I_VCMPNGEPS:
    case I_VCMPNGTPS:
    case I_VCMPFALSEPS:
    case I_VCMPNEQ_OQPS:
    case I_VCMPGEPS:
    case I_VCMPGTPS:
    case I_VCMPTRUEPS:
    case I_VCMPEQ_OSPS:
    case I_VCMPLT_OQPS:
    case I_VCMPLE_OQPS:
    case I_VCMPUNORD_SPS:
    case I_VCMPNEQ_USPS:
    case I_VCMPNLT_UQPS:
    case I_VCMPNLE_UQPS:
    case I_VCMPORD_SPS:
    case I_VCMPEQ_USPS:
    case I_VCMPNGE_UQPS:
    case I_VCMPNGT_UQPS:
    case I_VCMPFALSE_OSPS:
    case I_VCMPNEQ_OSPS:
    case I_VCMPGE_OQPS:
    case I_VCMPGT_OQPS:
    case I_VCMPTRUE_USPS:
    /* ohters */
    case I_PI2FD:
    case I_PI2FW:
    case I_PF2IW:
    case I_PF2ID:
    case I_PSWAPD:
    case I_VPBLENDVB:
    case I_PFNACC:
        opsiz++;
    }

    if (rel_disp->size > 0) {
        rel_disp->offset = di->size - opsiz + disp_offset;
        funchook_log(ctx->funchook, "      ip-relative %08x, absolute address= %0"SIZE_T_WIDTH SIZE_T_FMT"x, offset=%d, size=%d\n",
                     (uint32_t)rel_disp->raddr, (size_t)rel_disp->addr, rel_disp->offset, rel_disp->size);
    }
    if (rel_imm->size > 0) {
        rel_imm->offset = di->size - opsiz + imm_offset;
        funchook_log(ctx->funchook, "      ip-relative %08x, absolute address= %0"SIZE_T_WIDTH SIZE_T_FMT"x, offset=%d, size=%d\n",
                     (uint32_t)rel_imm->raddr, (size_t)rel_imm->addr, rel_imm->offset, rel_imm->size);
    }
}

/*
 * Fix RIP-relative address in an instruction
 */
static int handle_rip_relative(make_trampoline_context_t *ctx, const rip_relative_t *rel, const _DInst *di)
{
    if (rel->size == 32) {
        if (*(int32_t*)(ctx->dst + rel->offset) != (uint32_t)rel->raddr) {
            /* sanity check.
             * reach here if opsiz and/or disp_offset are incorrectly
             * estimated.
             */
            funchook_set_error_message(ctx->funchook, "Invalid ip-relative offset %d. The value at the offset should be %08x but %08x",
                         rel->offset, (uint32_t)rel->raddr, *(int32_t*)(ctx->dst + rel->offset));
            return FUNCHOOK_ERROR_IP_RELATIVE_OFFSET;
        }
        ctx->rip_disp[1].dst_addr = rel->addr;
        ctx->rip_disp[1].src_addr_offset = (ctx->dst - ctx->dst_base) + di->size;;
        ctx->rip_disp[1].pos_offset = (ctx->dst - ctx->dst_base) + rel->offset;
    } else if (rel->size != 0) {
        funchook_set_error_message(ctx->funchook, "Could not fix ip-relative address. The size is not 32.");
        return FUNCHOOK_ERROR_CANNOT_FIX_IP_RELATIVE;
    }
    return 0;
}
