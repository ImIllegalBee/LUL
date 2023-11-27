#pragma once

#include "BaseLUL.h"
#include "EntryPoint/Win64/EntryPoint.hpp"

class Application : public LUL::IApplication
{
public:

    Application() :
        IApplication(L"EmptyProject",
                     L"EmptyClassProject")
    {}

    ~Application() = default;

public:

    virtual void Init() override;

    virtual void Update() override;

    virtual void Destroy() override;

};