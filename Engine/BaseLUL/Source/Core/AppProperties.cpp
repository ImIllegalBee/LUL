#include "BaseLUL.h"

#ifdef _WIN64
#include <shlobj_core.h>
#endif // _WIN64

LUL::AppProperties::AppProperties(IN const std::wstring& appName, IN const std::wstring& appClass)
{
    if (appName.length() > m_AppName.length() ||
        appClass.length() > m_AppClass.length())
    {
        throw std::invalid_argument("AppProperties: appName or appClass argument is too long");
    }

    m_AppName = appName;
    m_AppClass = appClass;

    FindCreatePathLocalAppData();
    FindPathCurentDir();
}

bool LUL::AppProperties::MakeFileCurDir(IN const std::wstring& path) const
{
    return FindOrCreate(( m_AppCurPath + path ));
}

bool LUL::AppProperties::MakeFileAppData(IN const std::wstring& path) const
{
    return FindOrCreate(( m_AppDataPath + path ));
}

bool LUL::AppProperties::DoesExists(IN const std::wstring& path) const
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
                ( dwAttrib & FILE_ATTRIBUTE_NORMAL ) );
    }
    #endif // _WIN64

    LUL::Except::Internal(LUL_EXCEPT_INTERNAL_NOT_IMPL);
}

bool LUL::AppProperties::FindOrCreate(IN const std::wstring& path) const
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

bool LUL::AppProperties::MakeFile(IN const std::wstring& path) const
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

bool LUL::AppProperties::MakeDir(IN const std::wstring& path) const
{
    #ifdef _WIN64
    auto result = CreateDirectory(path.c_str(), NULL);
    
    // Operation was successful
    if (result)
        return result;

    LUL::Except::Win64::FromLastError();
    return;

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

