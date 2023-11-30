#pragma once

namespace LUL
{
    class AppProperties;

    struct LUL_DLL AppSettings
    {
    public:

        friend LUL::AppProperties;

    public:

        AppSettings(std::initializer_list<std::wstring> settings);

        AppSettings(std::wstring yourAppName, std::wstring yourAppClass) :
            AppSettings({ yourAppName, yourAppClass })
        {}

        ~AppSettings() = default;

    private:

        std::wstring AppName = std::wstring(LUL_STRING_V_SMALL, '\0');

        std::wstring AppClass = std::wstring(LUL_STRING_V_SMALL, '\0');

    };

    class LUL_DLL AppProperties
    {
    public:

        // Singleton

        static AppProperties* Get();

    private:

        AppProperties() = default;

    public:

        AppProperties(AppProperties&) = delete;

        void operator=(const AppProperties&) = delete;

        ~AppProperties() = default;

    public:
        
        /*
        * Initializes AppProperties and BaseLUL core components. */
        void Init(const AppSettings& yourSettings);

        /*
        * Create file/directory in '%CurrentDirectory%'
        * If the path ends with '\\' or '/' then it's treated as directory
        * else it's treated as file
        * @return True if the operation was successful or
        * path already existed */
        bool MakeFileCurDir(IN const std::wstring& path) const;

        /*
        * Create file/directory in '%AppData%/ImIllegalBee/'
        * If the path ends with '\\' or '/' then it's treated as directory
        * else it's treated as file
        * @return True if the operation was successful or
        * path already existed */
        bool MakeFileAppData(IN const std::wstring& path) const;

        /*
        * Check if the path exists.
        * If the path ends with '\\' or '/' then it's treated as directory
        * else it's treated as file
        * @return True if the path exists. */
        static bool DoesExists(IN const std::wstring& path);

        /*
        * Check if the path exists, if not then create it.
        * If the path ends with '\\' or '/' then it's treated as directory
        * else it's treated as file
        * @return True if whole operation was successful. */
        static bool FindOrCreate(IN const std::wstring& path);

    public:

        // Getters

        const std::wstring& GetAppName() const { return m_AppName; }

        const std::wstring& GetAppClass() const { return m_AppClass; }

        const std::wstring& GetAppBootTime() const { return m_AppBootTime; }

        const std::wstring& GetAppVersion() const { return m_AppVersion; }

        const std::wstring& GetAppDataPath() const { return m_AppDataPath; }

        const std::wstring& GetAppCurPath() const { return m_AppCurPath; }

    private:

        static bool MakeFile(IN const std::wstring& path);

        static bool MakeDir(IN const std::wstring& path);

        void FindCreatePathLocalAppData();

        void FindPathCurentDir();

    private:

        /*
        * Properties */
        std::wstring m_AppBootTime = {};
        std::wstring m_AppVersion = {};
        std::wstring m_AppName = {};
        std::wstring m_AppClass = {};

        /*
        * Paths */
        std::wstring m_AppDataPath = {};
        std::wstring m_AppCurPath = {};
    };
}