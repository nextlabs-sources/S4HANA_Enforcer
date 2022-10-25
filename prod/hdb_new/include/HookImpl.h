#ifndef HOOK_IMPL_H
#define HOOK_IMPL_H

#include "interface.h"
#ifndef __powerpc64__
#include "subhook.h"
#endif 
#include "funchook.h"

#if 0
class SubHookImpl : public IHook {
public:
    virtual int Prepare(void **old_addr, void *new_addr) override;
    virtual int Install();
};
#endif 

class FuncHookImpl : public IHook {
public:
    FuncHookImpl();
    virtual int Prepare(void **old_addr, void *new_addr) override;
    virtual int Install();

private:
    funchook_t* _funchook;
};


#endif