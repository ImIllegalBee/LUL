#pragma once

#include "BaseLUL.h"
#include "Interfaces/IInitialable.hpp"
#include "Interfaces/IWindow.hpp"

class SubWindow : public LUL::Win64::IEmptyWindow
{
public:

    SubWindow() {}

    ~SubWindow() {}

public:

    void Init() override;

    void Destroy() override
    {
        CallDestroy();
    }

    LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) override;

};