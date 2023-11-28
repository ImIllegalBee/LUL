#include "Application.hpp"

DECLARE_APPLICATION(Application);

void Application::Init()
{
    m_pLogg->SpawnSeparateThread();

    m_MainWindow.Init();

};

void Application::Update()
{
}

void Application::Destroy()
{
    m_pLogg->AddToLogQueue(LINFO, L"Destroy application");



    m_pLogg->KillSeparateThread();
}
