# Common global files

1. MemoryMgr

    1. Introduce
        1. Overload `new` and `delete` operators to record memory alloc and free info
        2. If you want to use this class to analysis memeory info, you just need to add this class to your project and compile.
        3. And invoke  `g_MemoryMgr.StartRecord();` at first.
        4. You can define 'DISABLE_OVERLOAD_NEW_DELETE' to disable if overload `new` and `delete` operators.
        5. If you want to show the file, function, line info in the memory info log, you can using the macro `NLNew` and `NLDelete` to instead operators `new` and `delete`.
    2. Problems:
        1. Do not support the memory alloc during global value construct before main/dllmain.