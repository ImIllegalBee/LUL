#pragma once

#include "BaseLUL.h"
#include "Interfaces/IInitialable.hpp"
#include "Interfaces/IWindow.hpp"

class MainWindow : public LUL::Win64::IEmptyWindow
{
public:

    MainWindow() {}

    ~MainWindow() {}

public:

    void Init() override;

    LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) override;

};