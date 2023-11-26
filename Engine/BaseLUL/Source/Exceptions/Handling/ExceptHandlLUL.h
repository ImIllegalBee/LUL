#pragma once

#define LUL_EXCEPT_INTERNAL_NOT_IMPL 0x01

namespace LUL::Except
{
    LUL_DLL inline void Internal(const int32_t& exceptCode);
}