#pragma once

namespace LUL::Except::Win64
{
    LUL_DLL inline void FromLastError(OPTIONAL OUT bool* isGood = nullptr);
}