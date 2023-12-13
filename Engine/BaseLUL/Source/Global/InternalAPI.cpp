#include "BaseLUL.h"

void LUL::ApiAddToLogQueue(IN const LogTags tag, IN const wchar_t* fmt, ...)
{
    wchar_t fmtBuff[ LUL_STRING_BIG ] = { 0 };

    va_list args;
    va_start(args, fmt);
    vswprintf_s(fmtBuff, fmt, args);
    va_end(args);

    // We can log on multiple threads only when the application is running
    if (LUL_IS_CORE_MULTITHREADED &&
        LUL::GetApp())
    {
        LUL::Logger::Get()->AddToLogQueue(tag, fmtBuff);
    }
    else
    {
        LUL::Logger::Get()->Log(tag, fmtBuff);
    }
}
