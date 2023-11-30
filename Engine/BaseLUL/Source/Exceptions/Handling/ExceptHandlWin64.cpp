#include "BaseLUL.h"

void LUL::Except::Win64::FromLastError(OPTIONAL OUT bool* isGood)
{
    DWORD error = GetLastError();

    switch (error)
    {
        case ( ERROR_ALREADY_EXISTS ):
        {
            // if (GetApp())
            //     GetApp()->GetLogger()->Log(LWARN, L"CreateDirectory: ERROR_ALREADY_EXISTS");

            if (isGood)
                *isGood = true;
            return;
        }

        case ( ERROR_PATH_NOT_FOUND ):
        {
            // if (GetApp())
            //     GetApp()->GetLogger()->Log(LWARN, L"CreateDirectory: ERROR_PATH_NOT_FOUND");

            if (isGood)
                *isGood = false;
            return;
        }

        default:
        {
            throw "THROW UNKOWN WIN64 EXECPTION";
        }
    }
}
