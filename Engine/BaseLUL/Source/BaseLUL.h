#pragma once

#ifdef _WIN64
    #ifndef WIN32_LEAN_AND_MEAN 
        #define WIN32_LEAN_AND_MEAN 
    #endif // !WIN32_LEAN_AND_MEAN 
    #include <Windows.h>
#endif

#ifdef _LUL_EXPORTS
    #define LUL_DLL __declspec(dllexport)
#else
    #define LUL_DLL __declspec(dllimport)
#endif // !_LUL_EXPORT

