#pragma once

#include "BaseLUL.h"
#include "EntryPoint/Win64/EntryPoint.hpp"
#include "Interfaces/IWindow.hpp"

#include "3DLUL.h"
#include "DirectX12/Renderer.hpp"

#include "Windows/MainWindow.hpp"
#include "Windows/SubWindow.hpp"

class Application : public LUL::IApplication
{
public:

    Application()
    {}

    ~Application() = default;

public:

    virtual void Init() override;

    virtual void Update() override;

    virtual void Destroy() override;

private:

    std::shared_ptr<LUL::Win64::DXRenderer> m_Renderer = nullptr;

private:

    std::shared_ptr<MainWindow> m_MainWindow = std::make_shared<MainWindow>();

    std::shared_ptr<SubWindow> m_SubWindow = std::make_shared<SubWindow>();

};