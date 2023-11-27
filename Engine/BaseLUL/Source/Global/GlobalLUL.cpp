#include "BaseLUL.h"

const LUL::IApplication* _pApp;

#pragma region LogFmt

wchar_t _LogDirFmt[ LUL_STRING_V_BIG ] = { 0 };
wchar_t _LogFileFmt[ LUL_STRING_V_BIG ] = { 0 };

LUL_DLL void LUL::SetApp(IApplication* pApp)
{
    if (!_pApp)
        _pApp = pApp;
    else
        LUL::Except::Internal(LUL_EXCEPT_INTERNAL_GLOBAL_SPACE_ACC_VIOL);
}

LUL_DLL const LUL::IApplication* LUL::GetApp()
{
    if (!_pApp)
        LUL::Except::Internal(LUL_EXCEPT_INTERNAL_GLOBAL_SPACE_NOT_INITIALIZED);

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

#pragma endregion
