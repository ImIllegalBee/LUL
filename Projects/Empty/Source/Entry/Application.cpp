#include "Application.hpp"

DECLARE_APPLICATION(Application);

void Application::Init()
{
    m_pLogg->SpawnSeparateThread();


};

void Application::Update()
{
    Sleep(1000);
}

void Application::Destroy()
{
    m_pLogg->AddToLogQueue(LINFO, L"Destroy application");



    m_pLogg->Destroy();
}
