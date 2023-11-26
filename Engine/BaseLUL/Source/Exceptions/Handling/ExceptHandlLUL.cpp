#include "BaseLUL.h"

void LUL::Except::Internal(const int32_t& exceptCode)
{
    switch (exceptCode)
    {
        case ( LUL_EXCEPT_INTERNAL_NOT_IMPL ):
        {
            // L_LOG(LERR, L"THROWING LUL_EXCEPT_INTERNAL_NOT_IMPL");
            throw "LUL_EXCEPT_INTERNAL_NOT_IMPL";
        }

        default:
        {
            // L_LOG(LERR, L"THROW UNKOWN EXECPTION");
            throw "THROW UNKOWN EXECPTION";
        }
    }
}
