#pragma once

namespace LUL::Except::Win64
{
    /*
    * @param isGood - tells how bad error was
    * and shows is program still good to run*/
    LUL_DLL inline void FromLastError(OPTIONAL OUT bool* isGood = nullptr);
}