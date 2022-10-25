#include "HookImpl.h"
#include "assert.h"

#if 0

int SubHookImpl::Install() {
    #if 0
    auto hk = new subhook::Hook((void*)oldf, (void*)newf, subhook::HookFlag64BitOffset);
    if (!hk->Install()) {
       // assert(false);
        return nullptr;
    }
    if (hk->GetTrampoline() == nullptr) {
        //assert(false);
        return nullptr;
    }
    return hk->GetTrampoline();
    #else
    return 0;
    #endif 
}
#endif 

FuncHookImpl::FuncHookImpl()
{
   _funchook = funchook_create();
}

int FuncHookImpl::Prepare(void **oldf, void *newf) {
    
    int rv = funchook_prepare(_funchook, oldf, newf);
    return rv;
}


int FuncHookImpl::Install() 
{  
    int rv   = funchook_install(_funchook, 0);
    return rv;
}

IHook *create_hook(IHook::HOOK_TYPE t) {
    switch (t) {
      //  case IHook::HOOK_BY_SUBHOOK: return new SubHookImpl;
        case IHook::HOOK_BY_FUNCHOOK: return new FuncHookImpl;
        default: return nullptr;
    }
}
