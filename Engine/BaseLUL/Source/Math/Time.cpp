#include "BaseLUL.h"

tm LUL::Time::TimeNow()
{
    tm myTime = {};
    time_t timeNow = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    localtime_s(&myTime, &timeNow);

    return myTime;
}

inline void TransformTimeToStr(int32_t time,
                               int32_t digitAmnt,
                               wchar_t* dest)
{
    // Start from zero index
    digitAmnt--;

    for (int64_t i = digitAmnt; i >= 0; i--)
    {
        int32_t tmpTime = time % 10;
        dest[ i ] = tmpTime + wchar_t('0');

        time /= 10;
    }
}

inline void PutFormat(wchar_t* dst,
                      size_t& dstSize,
                      size_t& iDst,
                      tm time,
                      const wchar_t* fmt,
                      size_t& iFmt)
{
    int32_t numValue = 0;

    switch (fmt[ iFmt + 1 ])
    {
        // Escape from the formatting
        case ( '%' ):
        {
            dst[ iDst ] = fmt[ iFmt ];
            iFmt++;

            return;
        }

        case ( 's' ):
        {
            numValue = time.tm_sec;
            break;
        }

        case ( 'm' ):
        {
            numValue = time.tm_min;
            break;
        }

        case ( 'h' ):
        {
            numValue = time.tm_hour;
            break;
        }

        case ( 'D' ):
        {
            numValue = time.tm_mday;
            break;
        }

        case ( 'M' ):
        {
            // Months are zero indexed
            numValue = time.tm_mon + 1;
            break;
        }

        case ( 'Y' ):
        {
            // Edge case

            TransformTimeToStr(( 1900 + time.tm_year ), 4, dst + iDst);

            // Skip 3 chars, because we just wrote 4 chars in to destination 
            // and every loop we increase indexes by one.
            iDst += 3;
            iFmt += 3;

            return;
        }
    }

    TransformTimeToStr(numValue, 2, dst + iDst);

    // Skip one additional char in both destination and format, because we just
    // added two chars, so we have to match the index.
    iDst++;
    iFmt++;
}

void LUL::Time::FmtToStr(OUT wchar_t* dest, IN size_t destSize, IN tm time, IN const wchar_t* format, IN const size_t formatSize)
{
    wchar_t* firstNonEmptyChr = wcschr(dest, L'\0');
    if (!firstNonEmptyChr)
        firstNonEmptyChr = dest;

    for (size_t dstI = 0, fmtI = 0;
         dstI < destSize && fmtI < formatSize;
         dstI++, fmtI++)
    {
        if (format[ fmtI ] == '\0')
        {
            firstNonEmptyChr[ dstI ] = '\0';
            return;
        }

        if (format[ fmtI ] == '%')
        {
            PutFormat(firstNonEmptyChr, destSize, dstI, time, format, fmtI);
            continue;
        }

        dest[ dstI ] = format[ fmtI ];
    }

    if (dest[ destSize - 1 ] != '\0')
        dest[ destSize - 1 ] = '\0';
}

size_t ConvertWcharsToInt(const wchar_t* str, int8_t amnt)
{
    size_t result = 0;

    for (int32_t i = 0; i < amnt; i++)
    {
        result *= 10;
        result += str[ i ] - L'0';
    }

    return result;
}

LUL_DLL size_t LUL::Time::CompareTwoDateStr(IN const wchar_t* a, IN const wchar_t* b, IN const wchar_t* fmt)
{
    size_t countA = 0;
    size_t countB = 0;

    for (int32_t i = 0;; i++)
    {
        if (fmt[ i ] == '\0' ||
            fmt[ i + 1 ] == '\0')
            break;

        if (fmt[ i ] != '%')
            continue;

        const wchar_t* pASubStrs = &a[ i ];
        const wchar_t* pBSubStrs = &b[ i ];

        switch (fmt[ i + 1 ])
        {
            // Escape from the formatting
            case ( '%' ):
            {
                continue;
            }

            case ( 's' ):
            {
                countA += ConvertWcharsToInt(pASubStrs, 2);
                countB += ConvertWcharsToInt(pBSubStrs, 2);

                continue;
            }

            case ( 'm' ):
            {
                countA += ConvertWcharsToInt(pASubStrs, 2) * 60;
                countB += ConvertWcharsToInt(pBSubStrs, 2) * 60;

                continue;
            }

            case ( 'h' ):
            {
                countA += ConvertWcharsToInt(pASubStrs, 2) * 60 * 60;
                countB += ConvertWcharsToInt(pBSubStrs, 2) * 60 * 60;

                continue;
            }

            case ( 'D' ):
            {
                countA += ConvertWcharsToInt(pASubStrs, 2) * 60 * 60 * 24;
                countB += ConvertWcharsToInt(pBSubStrs, 2) * 60 * 60 * 24;

                continue;
            }

            case ( 'M' ):
            {
                countA += ConvertWcharsToInt(pASubStrs, 2) * 60 * 60 * 24 * 30;
                countB += ConvertWcharsToInt(pBSubStrs, 2) * 60 * 60 * 24 * 30;

                continue;
            }

            case ( 'Y' ):
            {
                countA += ConvertWcharsToInt(pASubStrs + 1, 3) * 60 * 60 * 24 * 30 * 12;
                countB += ConvertWcharsToInt(pBSubStrs + 1, 3) * 60 * 60 * 24 * 30 * 12;

                continue;
            }
        }
    }

    return countA - countB;
}
