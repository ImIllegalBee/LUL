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
#endif // !_LUL_EXPORTS

#pragma region Standard lib includes
    #include <string>
    #include <stdexcept>
#pragma endregion 

#pragma region Engine definitions
    #define LUL_STRING_V_SMALL  32
    #define LUL_STRING_SMALL    128
    #define LUL_STRING_MEDIUM   256
    #define LUL_STRING_BIG      512
    #define LUL_STRING_V_BIG    1024
    
    #define LUL_PATH 260
#pragma endregion

#pragma region Compiler error suppression
#pragma warning ( push )
// Warning	C4251	class needs to have dll - interface to be used by clients of class 

#pragma warning ( disable : 4251)
#pragma endregion

#pragma region Engine includes
    #include "Exceptions/Handling/ExceptHandlLUL.h"
    #ifdef _WIN64
        #include "Exceptions/Handling/ExceptHandlWin64.h"
    #endif // _WIN64
    
    #include "Core/AppProperties.hpp"
#pragma endregion

#pragma warning ( pop )