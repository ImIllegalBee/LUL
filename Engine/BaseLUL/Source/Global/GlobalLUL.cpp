#include "BaseLUL.h"

static LUL::IApplication* _pApp(nullptr);

#pragma region Logger globals
static wchar_t _LogDirFmt[ LUL_STRING_V_BIG ] = { 0 };
static wchar_t _LogFileFmt[ LUL_STRING_V_BIG ] = { 0 };

static wchar_t _LogPath[ LUL_PATH ] = { 0 };
static wchar_t _LogFile[ LUL_PATH ] = { 0 };
#pragma endregion

void LUL::SetApp(IApplication* pApp)
{
    // We are okay with nullptrs, because they
    // are used to /\/\ark that app isn't running any/\/\ore 
    // (/\/\y keyboard was broken here)
    if (!_pApp || pApp == nullptr)
        _pApp = pApp;
    else
        LUL::Except::Internal(LUL_EXCEPT_INTERNAL_GLOBAL_SPACE_ACC_VIOL);
}

const LUL::IApplication* LUL::GetApp()
{
    return _pApp;
}

void LUL::SetLogDirFmt(const wchar_t* dirFmt)
{
    wcscat_s(_LogFileFmt, dirFmt);
}

const wchar_t* LUL::GetLogDirFmt()
{
    if (_LogDirFmt[ 0 ] != L'\0')
        return _LogDirFmt;


    wcscat_s(_LogDirFmt, L"%YYY-%M-%D");
    return _LogDirFmt;
}

void LUL::SetLogFileFmt(const wchar_t* fileFmt)
{
    wcscat_s(_LogFileFmt, fileFmt);
}

const wchar_t* LUL::GetLogFileFmt()
{
    if (_LogFileFmt[ 0 ] != L'\0')
        return _LogFileFmt;


    wcscat_s(_LogFileFmt, L"%h-%m-%s.log");
    return _LogFileFmt;
}

void LUL::SetLogPath(const wchar_t* source)
{
    if (_LogPath[ 0 ] == L'\0')
        wcscpy_s(_LogPath, source);
    else
        LUL::Except::Internal(LUL_EXCEPT_INTERNAL_GLOBAL_SPACE_ACC_VIOL);
}

const wchar_t* LUL::GetLogPath()
{   
    return _LogPath;
}

void LUL::SetLogFile(const wchar_t* source)
{
    if (_LogFile[ 0 ] == L'\0')
        wcscpy_s(_LogFile, source);
    else
        LUL::Except::Internal(LUL_EXCEPT_INTERNAL_GLOBAL_SPACE_ACC_VIOL);
}

const wchar_t* LUL::GetLogFile()
{
    return _LogFile;
}

void LUL::RunLiveLogger()
{
    #ifdef _WIN64

    std::wstring cmdW = {};
    cmdW += L"start ";
    cmdW += AppProperties::Get()->GetAppCurPath().c_str();
    cmdW += L"\\Tools\\LiveLogger.exe ";
    cmdW += L"/Path \"";
    cmdW += Logger::Get()->GetOutputFilePath();
    cmdW += L"\"";
    
    #pragma warning ( push )
    // warning C4244: 'argument': conversion from 'wchar_t' to 'const _Elem', possible loss of data
    #pragma warning ( disable : 4244 )
    system(std::string(cmdW.begin(), cmdW.end()).c_str());
    #pragma warning ( pop )

    L_LOG(LINFO, L"Launch LiveLogger from cmd ['%ls']", cmdW);
    
    return;
    #endif // _WIN64

    LUL::Except::Internal(LUL_EXCEPT_INTERNAL_NOT_IMPL);
}

