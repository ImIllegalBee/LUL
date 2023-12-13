#pragma once

#include "BaseLUL.h"

#include "Interfaces/IApplication.hpp"

#ifdef _WIN64

INT CALLBACK WinMain(HINSTANCE hInst, HINSTANCE hInstPrev, PSTR cmdline, INT cmdshow)
{
    auto* app = LUL::EntryApplication();
    LUL::SetApp(app);
    
    if (!LUL::GetApp())
    {
        MessageBox(NULL, L"Application was null!", L"Error at initialization!", MB_ICONEXCLAMATION | MB_OK);
        return -1;
    }

    app->Init();

    MSG msg = { 0 };
    while (msg.message != WM_QUIT)
    {
        if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        app->Update();
    }

    app->Destroy();

    // Application isn't running anymore
    LUL::SetApp(nullptr);

    // Destroy singletons
    {
        LUL::Logger::Get()->~Logger();
        LUL::AppProperties::Get()->~AppProperties();
    }

    return 0;
}

#endif // _WIN64