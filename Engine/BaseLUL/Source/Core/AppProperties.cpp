#include "BaseLUL.h"

#ifdef _WIN64
#include <shlobj_core.h>
#endif // _WIN64

#include "Logger.hpp"

LUL::AppSettings::AppSettings(std::initializer_list<std::wstring> settings)
{
    for (size_t i = 0; i < settings.size(); i++)
    {
        std::wstring arg = *( settings.begin() + i );

        switch (i)
        {
            case ( 0 ):
            {
                AppName = arg.substr(0, AppName.length() - 1);

                continue;
            }

            case ( 1 ):
            {
                AppClass = arg.substr(0, AppClass.length() - 1);

                continue;
            }

            default:
                return;
        }
    }
}

// AppProperties --------------------------------------------------------------

LUL::AppProperties* LUL::AppProperties::Get()
{
    static AppProperties m_Instance;
    return &m_Instance;
}

void LUL::AppProperties::InitCoreLULComponents(const AppSettings& yourSettings)
{
    // It's already initialized
    if (!m_AppName.empty())
        return;

    m_AppName = yourSettings.AppName;
    m_AppClass = yourSettings.AppClass;
    yourSettings.~AppSettings();

    FindCreatePathLocalAppData();
    FindPathCurentDir();

    Logger::Get();
}

bool LUL::AppProperties::MakeFileCurDir(IN const std::wstring& path) const
{
    if (!m_AppCurPath.length())
        LUL::Except::Internal(LUL_EXCEPT_INTERNAL_INIT_ERR);

    return FindOrCreate(( m_AppCurPath + path ));
}

bool LUL::AppProperties::MakeFileAppData(IN const std::wstring& path) const
{
    if (!m_AppDataPath.length())
        LUL::Except::Internal(LUL_EXCEPT_INTERNAL_INIT_ERR);

    return FindOrCreate(( m_AppDataPath + path ));
}

bool LUL::AppProperties::DoesExists(IN const std::wstring& path)
{
    if (path.length() >= LUL_PATH)
        throw std::invalid_argument("AppProperties: DoesExists: path too long");

    #ifdef _WIN64
    DWORD dwAttrib = GetFileAttributes(path.c_str());

    if (path.back() == L'\\' ||
        path.back() == L'/')
    {
        return ( dwAttrib != INVALID_FILE_ATTRIBUTES &&
                ( dwAttrib & FILE_ATTRIBUTE_DIRECTORY ) );
    }
    else
    {
        return ( dwAttrib != INVALID_FILE_ATTRIBUTES &&
                ( dwAttrib &~ FILE_ATTRIBUTE_DIRECTORY ));
    }
    #endif // _WIN64

    LUL::Except::Internal(LUL_EXCEPT_INTERNAL_NOT_IMPL);
}

bool LUL::AppProperties::FindOrCreate(IN const std::wstring& path)
{
    if (path.length() >= LUL_PATH)
        throw std::invalid_argument("AppProperties: FindOrCreate: path too long");

    if (path.back() == L'\\' ||
        path.back() == L'/')
    {
        // We are talking about directory
        if (DoesExists(path))
            return true;

        return MakeDir(path);
    }
    else
    {
        // It's a file
        if (DoesExists(path))
            return true;

        return MakeFile(path);
    }
}

// PRIVATE --------------------------------------------------------------------

bool LUL::AppProperties::MakeFile(IN const std::wstring& path)
{
    #ifdef _WIN64
    HANDLE file = CreateFile(path.c_str(),
                             GENERIC_READ | GENERIC_WRITE,
                             0,
                             NULL,
                             CREATE_NEW,
                             FILE_ATTRIBUTE_NORMAL,
                             NULL);

    if (file == INVALID_HANDLE_VALUE)
        return false;

    CloseHandle(file);

    return true;
    #endif // _WIN64

    LUL::Except::Internal(LUL_EXCEPT_INTERNAL_NOT_IMPL);
}

bool LUL::AppProperties::MakeDir(IN const std::wstring& path)
{
    #ifdef _WIN64
    auto result = CreateDirectory(path.c_str(), NULL);

    // Operation was successful
    if (result)
        return result;

    bool isGood = false;
    LUL::Except::Win64::FromLastError(&isGood);
    return isGood;

    #endif // _WIN64

    LUL::Except::Internal(LUL_EXCEPT_INTERNAL_NOT_IMPL);
}

void LUL::AppProperties::FindCreatePathLocalAppData()
{
    #ifdef _WIN64
    PWSTR appDataPath = nullptr;
    SHGetKnownFolderPath(FOLDERID_RoamingAppData,
                         0,
                         nullptr,
                         &appDataPath);

    // Copy path, free the memory, add suffix
    m_AppDataPath = appDataPath;
    CoTaskMemFree(appDataPath);
    m_AppDataPath += L"\\ImIllegalBee\\";

    MakeDir(m_AppDataPath);

    return;
    #endif // _WIN64

    LUL::Except::Internal(LUL_EXCEPT_INTERNAL_NOT_IMPL);
}

void LUL::AppProperties::FindPathCurentDir()
{
    #ifdef _WIN64
    wchar_t* tmpFileName = (wchar_t*) malloc(sizeof(wchar_t) * LUL_PATH);
    if (!tmpFileName)
        throw std::bad_alloc();

    GetModuleFileName(NULL, tmpFileName, LUL_PATH);

    wchar_t* lastSlash = wcsrchr(tmpFileName, L'\\');
    *( lastSlash + 1 ) = L'\0';

    m_AppCurPath = tmpFileName;
    free(tmpFileName);

    return;
    #endif // _WIN64

    LUL::Except::Internal(LUL_EXCEPT_INTERNAL_NOT_IMPL);
}

