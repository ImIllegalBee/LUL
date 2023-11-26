#pragma once

namespace LUL
{
    class LUL_DLL AppProperties
    {
    public:

        AppProperties(IN const std::wstring& appName,
                      IN const std::wstring& appClass);

        AppProperties(IN const std::wstring& appName) :
            AppProperties(appName, L"default class")
        {}

        AppProperties() :
            AppProperties(L"default name", L"default class")
        {}

        ~AppProperties() = default;

    public:
        
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
        bool DoesExists(IN const std::wstring& path) const;

        /*
        * Check if the path exists, if not then create it.
        * If the path ends with '\\' or '/' then it's treated as directory
        * else it's treated as file
        * @return True if whole operation was successful. */
        bool FindOrCreate(IN const std::wstring& path) const;

    public:

        // Getters

        const std::wstring& GetAppName() const { return m_AppName; }

        const std::wstring& GetAppClass() const { return m_AppClass; }

        const std::wstring& GetAppBootTime() const { return m_AppBootTime; }

        const std::wstring& GetAppVersion() const { return m_AppVersion; }

        const std::wstring& GetAppDataPath() const { return m_AppDataPath; }

        const std::wstring& GetAppCurPath() const { return m_AppCurPath; }

    private:

        bool MakeFile(IN const std::wstring& path) const;

        bool MakeDir(IN const std::wstring& path) const;

        void FindCreatePathLocalAppData();

        void FindPathCurentDir();

    private:

        /*
        * Properties */
        std::wstring m_AppBootTime = {};
        std::wstring m_AppVersion = {};
        // Max size for m_AppName and m_AppClass is LUL_STRING_V_SMALL
        std::wstring m_AppName = std::wstring(LUL_STRING_V_SMALL, '\0');
        std::wstring m_AppClass = std::wstring(LUL_STRING_V_SMALL, '\0');

        /*
        * Paths */
        std::wstring m_AppDataPath = {};
        std::wstring m_AppCurPath = {};
    };
}