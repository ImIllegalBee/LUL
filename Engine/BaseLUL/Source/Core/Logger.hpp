#pragma once

#pragma region Log tag macros
#define LPLAIN LUL::LogTags::Plain
#define LINFO LUL::LogTags::Info
#define LMESS LUL::LogTags::Message
#define LWARN LUL::LogTags::Warning
#define LERR LUL::LogTags::Error
#define LCORR LUL::LogTags::Coruption
#pragma endregion

#ifdef _DEBUG
    #define L_LOG(tag, fmt, ...) 
#else
    #define L_LOG(tag, fmt, ...) 
#endif // _DEBUG

namespace LUL
{
    enum LUL_DLL LogTags
    {
        PlainText,
        Info,
        Message,
        Warning,
        Error,
        Coruption
    };

    class LUL_DLL Logger
    {
    public:

        Logger(IN const std::shared_ptr<LUL::AppProperties>& appCfg,
               IN const bool& isMultiThreaded = true);

        ~Logger();

    public:

        /*
        * Single thread logging */
        void Log(IN const LogTags tag, IN const wchar_t* fmt, ...) const;

        /*
        * Adds to logging queue on second thread. */
        void AddToLogQueue(IN const LogTags tag, IN const wchar_t* str) const;

        void RedoLastLog() const;

        /*
        * Logging on separate thread*/
        void SpawnSeparateThread();

        void KillSeparateThread();

    private:

        /*
        * Clean old files */
        void CleanOldFiles(IN const std::shared_ptr<LUL::AppProperties> appCfg);

        /*
        * Clean old file threaded, called in constructor */
        void CleanOldFileThreaded(IN const std::shared_ptr<LUL::AppProperties>& appCfg);

        void FmtStrFromTag(OUT wchar_t* str,
                           IN const LogTags& tag,
                           OPTIONAL IN const tm when = LUL::Time::TimeNow()) const;

        void LoggingLoop();

    private:

        std::wstring m_LogPath = {};
        std::wstring m_LogFileName = {};

        // If any of them are '0' then the check in CleaOldFiles is skipped.
        int32_t m_FileLifeSpanDays = 1;
        int32_t m_FileLifeSpanHours = 0;

        // Used to retry logging in case of errors
        wchar_t m_LastBrokenBuff[ LUL_STRING_V_BIG ] = { 0 };

        std::thread m_CleanOldThread = {};

        std::atomic_bool m_UseSeparateThread = false;
        std::shared_ptr<std::thread> m_SeparateThread = std::make_shared<std::thread>();
        std::shared_ptr<std::queue<std::tuple<tm, LogTags, std::wstring>>> m_SepareteThreadFIFOQueue = \
            std::make_shared<std::queue<std::tuple<tm, LogTags, std::wstring>>>();

    };
}
