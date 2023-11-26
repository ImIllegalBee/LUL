#include "BaseLUL.h"

void LUL::Except::Win64::FromLastError()
{
    DWORD error = GetLastError();

    switch (error)
    {
        case ( ERROR_ALREADY_EXISTS ):
        {
            // L_Log(LWARN, L"CreateDirectory: ERROR_ALREADY_EXISTS");

            return;
        }

        case ( ERROR_PATH_NOT_FOUND ):
        {
            // L_Log(LWARN, L"CreateDirectory: ERROR_PATH_NOT_FOUND");

            return;
        }

        default:
        {
            throw "THROW UNKOWN WIN64 EXECPTION";
        }
    }
}
