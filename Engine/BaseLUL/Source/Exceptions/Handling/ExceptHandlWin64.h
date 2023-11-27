#pragma once

namespace LUL::Except::Win64
{
    /*
    * @param isGood - tells how bad was error was
    * and is program still good to run*/
    LUL_DLL inline void FromLastError(OPTIONAL OUT bool* isGood = nullptr);
}