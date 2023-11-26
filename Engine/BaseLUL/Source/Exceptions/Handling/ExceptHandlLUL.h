#pragma once

#define LUL_EXCEPT_INTERNAL_NOT_IMPL 0x01

#define LUL_EXCEPT_INTERNAL_LOG_ACCESS_VIOL 0x30

namespace LUL::Except
{
    LUL_DLL inline void Internal(const int32_t& exceptCode);
}