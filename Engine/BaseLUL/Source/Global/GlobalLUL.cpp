#include "BaseLUL.h"

static LUL::IApplication* _pApp(nullptr);

#pragma region Logging formats

wchar_t _LogDirFmt[ LUL_STRING_V_BIG ] = { 0 };
wchar_t _LogFileFmt[ LUL_STRING_V_BIG ] = { 0 };

#pragma endregion

void LUL::SetApp(IApplication* pApp)
{
    if (!_pApp)
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

