#pragma once

namespace LUL
{
    LUL_DLL void SetApp(IApplication* pApp);

    LUL_DLL const LUL::IApplication* GetApp();

    LUL_DLL void SetLogDirFmt(const wchar_t* dirFmt);

    LUL_DLL const wchar_t* GetLogDirFmt();

    LUL_DLL void SetLogFileFmt(const wchar_t* fileFmt);

    LUL_DLL const wchar_t* GetLogFileFmt();

    LUL_DLL void SetLogPath(const wchar_t* source);

    LUL_DLL const wchar_t* GetLogPath();

    LUL_DLL void SetLogFile(const wchar_t* source);

    LUL_DLL const wchar_t* GetLogFile();

    LUL_DLL void RunLiveLogger();
}