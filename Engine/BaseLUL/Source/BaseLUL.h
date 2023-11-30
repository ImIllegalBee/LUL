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
    #include <memory>
    #include <queue>
    #include <fstream>
    #include <chrono>
    #include <thread>
    #include <filesystem>
#pragma endregion 

#pragma region Engine definitions
    #define LUL_STRING_V_SMALL  32
    #define LUL_STRING_SMALL    128
    #define LUL_STRING_MEDIUM   256
    #define LUL_STRING_BIG      512
    #define LUL_STRING_V_BIG    1024
    
    #define LUL_PATH 260

    // Unit conversion 
    #define SECONDS_TO_MILLISECONDS(x) (x * 1000)
    #define SECONDS_TO_NANOSECONDS(x) (x * 1000000000)
    
    #define MILLISECONDS_TO_SECONDS(x) (x / 1000)
    #define MILLISECONDS_TO_NANOSECONDS(x) (x * 1000000)
    
    #define NANOSECONDS_TO_SECONDS(x) (x / 1000000000)
    #define NANOSECONDS_TO_MILLISECONDS(x) (x / 1000000)
    
    #define MEM_KB(x)   (x * 1024)
    #define MEM_MB(x)   MEM_KB(x * 1024)

    /* 
    * Controls if BaseLUL should use multithreaded
    * versions of core components */
    #define LUL_IS_CORE_MULTITHREADED true

#ifdef _WIN64
    #define	HInstance() GetModuleHandle(NULL)
#endif // _WIN64

#pragma endregion

#pragma region Compiler warnings suppression
    #pragma warning ( push )
    // Warning C4251 class needs to have dll - interface to be used by clients of class 
    #pragma warning ( disable : 4251)
#pragma endregion

#pragma region Engine includes
    #include "Exceptions/Handling/ExceptHandlLUL.h"
    #ifdef _WIN64
        #include "Exceptions/Handling/ExceptHandlWin64.h"
    #endif // _WIN64

    #include "Interfaces/IUnknown.hpp"
    #include "Interfaces/IInitialable.hpp"
    #include "Interfaces/IDestroyable.hpp"
    #include "Interfaces/IApplication.hpp"

    #include "Math/Time.hpp"

    #include "Core/Types.hpp"
    #include "Core/AppProperties.hpp"
    #include "Core/Logger.hpp"

    #include "Global/GlobalLUL.hpp"
    #include "Global/InternalAPI.hpp"
#pragma endregion

#pragma warning ( pop )