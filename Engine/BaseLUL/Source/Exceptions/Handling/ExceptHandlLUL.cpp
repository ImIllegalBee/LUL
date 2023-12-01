#include "BaseLUL.h"

void LUL::Except::Internal(const int32_t& exceptCode)
{
    switch (exceptCode)
    {
        case ( LUL_EXCEPT_INTERNAL_NOT_IMPL ):
        {
            L_LOG(LERR, L"THROWING LUL_EXCEPT_INTERNAL_NOT_IMPL");
            throw "LUL_EXCEPT_INTERNAL_NOT_IMPL";
        }

        case ( LUL_EXCEPT_INTERNAL_LOG_ACCESS_VIOL ):
        {
            using namespace std::chrono_literals;

            static int16_t retries = 0;

            if (retries >= 25)
            {
                #ifdef _WIN64
                // Throw message box with error()
                #endif // _WIN64

                throw;
            }
            int16_t currentInstanceRetries = ++retries;

            std::this_thread::sleep_for(500ms);

            Logger::Get()->RedoLastLog();

            if (currentInstanceRetries != retries)
                return;

            L_LOG(LWARN, L"LUL_EXCEPTION_ACCESS_VIOLATION_LOGGER_FILE succeded after %d retry/ies", retries);
            retries = 0;
            return;
        }

        default:
        {
            L_LOG(LERR, L"THROW UNKOWN EXECPTION");
            throw "THROW UNKOWN EXECPTION";
        }
    }
}
