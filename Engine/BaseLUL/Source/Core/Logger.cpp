#include "BaseLUL.h"

LUL::Logger::Logger(IN const std::shared_ptr<LUL::AppProperties>& appCfg,
                    IN const bool& isMultiThreaded)
{
    namespace llt = LUL::Time;

    // Create Logs dir in appdata
    m_LogPath += L"Logs\\";
    if (!appCfg->MakeFileAppData(m_LogPath))
        LUL::Except::Internal(LUL_EXCEPT_INTERNAL_INIT_ERR);

    // Create dir with current day as name in appdata
    wchar_t dirName[ LUL_STRING_V_SMALL ] = { 0 };
    llt::FmtToStr(dirName,
                  llt::TimeNow(),
                  LUL::GetLogDirFmt(),
                  LUL_STRING_V_SMALL);
    m_LogPath += dirName;
    m_LogPath += L"\\";
    if (!appCfg->MakeFileAppData(m_LogPath))
        LUL::Except::Internal(LUL_EXCEPT_INTERNAL_INIT_ERR);

    // Create file in that dir
    wchar_t file[ LUL_STRING_V_SMALL ] = { 0 };
    llt::FmtToStr(file,
                  llt::TimeNow(),
                  LUL::GetLogFileFmt(),
                  LUL_STRING_V_SMALL);
    m_LogFileName += file;

    std::wstring tmpFullPath = {};
    tmpFullPath += m_LogPath;
    tmpFullPath += m_LogFileName;
    if (!appCfg->MakeFileAppData(tmpFullPath))
        LUL::Except::Internal(LUL_EXCEPT_INTERNAL_INIT_ERR);

    m_LogPath = appCfg->GetAppDataPath();
    m_LogPath += tmpFullPath;


    if (isMultiThreaded)
        CleanOldFileThreaded(appCfg);
    else
        CleanOldFiles(appCfg);
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

    std::wofstream outFile(m_LogPath,
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

void LUL::Logger::SpawnSeparateThread()
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

void LUL::Logger::KillSeparateThread()
{
    m_UseSeparateThread.store(false);
}

// PRIVATE --------------------------------------------------------------------

void LUL::Logger::CleanOldFiles(IN const std::shared_ptr<LUL::AppProperties> appCfg)
{
    namespace fs = std::filesystem;

    wchar_t dir[ LUL_PATH ] = {};

    wcscat_s(dir, appCfg->GetAppDataPath().c_str());
    wcscat_s(dir, L"Logs");

    wchar_t dateNow[ LUL_STRING_V_SMALL ] = { 0 };
    LUL::Time::FmtToStr(dateNow,
                        LUL::Time::TimeNow(),
                        LUL::GetLogDirFmt(),
                        LUL_STRING_V_SMALL);

    size_t deleteTreshold;

    // Delete on days basis
    if (m_FileLifeSpanDays)
    {
        deleteTreshold = m_FileLifeSpanDays * 24 * 60 * 60;

        for (auto& itm : fs::directory_iterator(dir))
        {
            if (!itm.is_directory())
                continue;

            const wchar_t* fileName = wcsrchr(itm.path().c_str(), L'\\') + 1;

            size_t diff = LUL::Time::CompareTwoDateStr(dateNow, fileName, LUL::GetLogDirFmt());

            if (diff > deleteTreshold)
            {
                L_LOG(LINFO, L"Removing log dir: %ls", (const wchar_t*) itm.path().c_str());
                fs::remove_all(itm.path());
            }
        }
    }

    if (!m_FileLifeSpanHours)
        return;

    deleteTreshold = m_FileLifeSpanHours * 60 * 60;

    wchar_t* lastSlash = wcsrchr(dir, L'\\');
    *( lastSlash + 1 ) = L'\0';

    wcscat_s(dir, m_LogPath.c_str());

    // Delete on hours basis
    for (auto& itm : fs::directory_iterator(dir))
    {
        if (itm.is_directory())
            continue;

        const wchar_t* fileName = wcsrchr(itm.path().c_str(), L'\\') + 1;

        size_t diff = LUL::Time::CompareTwoDateStr(dateNow, fileName, LUL::GetLogFileFmt());

        if (diff > deleteTreshold)
        {
            L_LOG(LINFO, L"Removing log file: %ls", (const wchar_t*) itm.path().c_str());
            remove(itm.path());
        }
    }
}

void LUL::Logger::CleanOldFileThreaded(IN const std::shared_ptr<LUL::AppProperties>& appCfg)
{
    if (m_CleanOldThread.joinable())
    {
        Log(LogTags::Warning, L"m_CleanOldThread is already running!");
        return;
    }

    std::thread newThread(&LUL::Logger::CleanOldFiles, this, appCfg);
    m_CleanOldThread.swap(newThread);
    newThread.~thread();
}

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

        std::wofstream logFile(m_LogPath,
                               std::ios_base::out | std::ios_base::app);

        if (!logFile.is_open())
            continue;

        logFile << tagBuff << std::get<2>(first) << L"\n";

        logFile.close();

        m_SepareteThreadFIFOQueue->pop();
    }
}
