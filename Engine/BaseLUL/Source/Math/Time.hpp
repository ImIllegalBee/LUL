#pragma once

namespace LUL::Time
{
    LUL_DLL inline tm TimeNow();

    #pragma region FmtToStr

    /*
    * Formats string with time values
    * Copies every non format char
    * Destination doesn't have to be empty, 
    * format appends to string
    * %%   - escapes format, places percent symbol
    * %s   - seconds
    * %m   - minutes
    * %h   - hours
    * %D   - days
    * %M   - months
    * %YYY - years */
    LUL_DLL inline void FmtToStr(OUT wchar_t* dest,
                                 IN size_t destSize,
                                 IN tm time,
                                 IN const wchar_t* format,
                                 IN const size_t formatSize);

    template <size_t destSize>
    LUL_DLL inline void FmtToStr(OUT wchar_t(&dest)[ destSize ],
                                 IN tm time,
                                 IN const wchar_t* format,
                                 IN const size_t formatSize)
    {
        return LUL::Time::FmtToStr(dest,
                                    destSize,
                                    time,
                                    format,
                                    formatSize);
    }

    template <size_t formatSize>
    LUL_DLL inline void FmtToStr(OUT wchar_t* dest,
                                 IN size_t destSize,
                                 IN tm time,
                                 IN wchar_t(&format)[ formatSize ])
    {
        return LUL::Time::FmtToStr(dest,
                                    destSize,
                                    time,
                                    format,
                                    formatSize);
    }


    template <size_t destSize, size_t formatSize>
    LUL_DLL inline void FmtToStr(OUT wchar_t(&dest)[ destSize ],
                                 IN tm time,
                                 IN wchar_t(&format)[ formatSize ])
    {
        return LUL::Time::FmtToStr(dest,
                                    destSize,
                                    time,
                                    format,
                                    formatSize);
    }

    #pragma endregion
}