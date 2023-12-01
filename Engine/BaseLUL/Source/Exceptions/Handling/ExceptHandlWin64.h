#pragma once

#ifdef _WIN64

namespace LUL::Except::Win64
{
    /*
    * @param isGood - tells how bad error was
    * and shows is program still good to run*/
    LUL_DLL inline void FromLastError(OPTIONAL OUT bool* isGood = nullptr);

    LUL_DLL inline void ThrowIfFailed(IN HRESULT result);
}

#endif // _WIN64
