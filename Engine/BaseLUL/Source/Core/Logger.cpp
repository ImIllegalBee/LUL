#include "BaseLUL.h"

LUL::Logger::Logger(IN const std::shared_ptr<LUL::AppProperties>& appCfg)
{

}

LUL::Logger::~Logger()
{
    if (m_UseSeparateThread.load() ||
        m_SeparateThread->joinable())
    {
        m_UseSeparateThread.store(false);

        m_SeparateThread->join();
    }
}

void LUL::Logger::Log(IN const LogTags tag, IN const wchar_t* fmt, ...) const
{
    wchar_t fmtBuff[ LUL_STRING_BIG ] = { 0 };

    va_list args;
    va_start(args, fmt);
    vswprintf_s(fmtBuff, fmt, args);
    va_end(args);

    wchar_t tagStr[ LUL_STRING_V_SMALL ] = { 0 };
    FmtStrFromTag(tagStr, tag);

    std::wofstream outFile(m_LogDirPath,
                           std::ios_base::out | std::ios_base::app);

    if (!outFile.is_open())
    {
        // Hacky, because this function is class const, but it works for me
        wcscpy_s((wchar_t*) m_LastBrokenBuff, LUL_STRING_BIG, tagStr);
        wcscpy_s((wchar_t*) m_LastBrokenBuff, LUL_STRING_BIG, fmtBuff);

        return LUL::Except::Internal(LUL_EXCEPT_INTERNAL_LOG_ACCESS_VIOL);
    }

    outFile << tagStr << fmtBuff << L"\n";
    outFile.close();
}

void LUL::Logger::AddToLogQueue(IN const LogTags tag, IN const wchar_t* str) const
{
    m_SepareteThreadFIFOQueue->push(std::make_tuple(LUL::Time::TimeNow(), tag, str));
}

void LUL::Logger::RedoLastLog() const
{
    Log(PlainText, m_LastBrokenBuff);
}

void LUL::Logger::SpawnLoggingThread()
{
    if (m_UseSeparateThread.load() ||
        m_SeparateThread->joinable())
    {
        AddToLogQueue(LogTags::Warning, L"Separate thread for logger is already on!");

        return;
    }

    m_UseSeparateThread.store(true);
    std::thread newThread(&LUL::Logger::LoggingLoop, this);
    m_SeparateThread->swap(newThread);
    newThread.~thread();
}

// PRIVATE --------------------------------------------------------------------

void LUL::Logger::CleanOldLogs(IN const std::shared_ptr<LUL::AppProperties> appCfg)
{}

void LUL::Logger::CleanOldLogsThreaded(IN const std::shared_ptr<LUL::AppProperties>& appCfg)
{}

void LUL::Logger::FmtStrFromTag(OUT wchar_t* str, IN const LogTags& tag, OPTIONAL IN const tm when) const
{
    if (tag == PlainText)
        return;

    const int32_t sizeOfBuff = LUL_STRING_V_SMALL;

    wcscat_s(str, sizeOfBuff, L"[ ");
    LUL::Time::FmtToStr(str + 2, sizeOfBuff, when, L"%h:%m:%s", 11);
    wcscat_s(str, sizeOfBuff, L" ][ ");
    switch (tag)
    {
        case ( Info ):
        {
            wcscat_s(str, sizeOfBuff, L"Info");
            break;
        }

        case ( Warning ):
        {
            wcscat_s(str, sizeOfBuff, L"Warning");
            break;
        }

        case ( Error ):
        {
            wcscat_s(str, sizeOfBuff, L"Error");
            break;
        }

        case ( Coruption ):
        {
            wcscat_s(str, sizeOfBuff, L"Coruption");
            break;
        }

        case ( Message ):
        {
            wcscat_s(str, sizeOfBuff, L"Message");
            break;
        }

        default:
        {
            break;
        }
    }
    wcscat_s(str, sizeOfBuff, L" ] ");
}

void LUL::Logger::LoggingLoop()
{
    while (m_UseSeparateThread.load())
    {
        if (m_SepareteThreadFIFOQueue->empty())
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            continue;
        }

        auto& first = m_SepareteThreadFIFOQueue->front();

        wchar_t tagBuff[ LUL_STRING_SMALL ] = { 0 };

        FmtStrFromTag(tagBuff, std::get<1>(first), std::get<0>(first));

        std::wofstream logFile(m_LogDirPath,
                               std::ios_base::out | std::ios_base::app);

        if (!logFile.is_open())
            continue;

        logFile << tagBuff << std::get<2>(first) << L"\n";

        logFile.close();

        m_SepareteThreadFIFOQueue->pop();
    }
}
