#include "BaseLUL.h"

void LUL::Except::Win64::FromLastError(OPTIONAL OUT bool* isGood)
{
    DWORD error = GetLastError();

    switch (error)
    {
        case ( ERROR_ALREADY_EXISTS ):
        {
            L_LOG(LWARN, L"CreateDirectory: ERROR_ALREADY_EXISTS");

            *isGood = true;
            return;
        }

        case ( ERROR_PATH_NOT_FOUND ):
        {
            L_LOG(LWARN, L"CreateDirectory: ERROR_PATH_NOT_FOUND");

            *isGood = false;
            return;
        }

        default:
        {
            throw "THROW UNKOWN WIN64 EXECPTION";
        }
    }
}
