#include "Application.hpp"

DECLARE_APPLICATION(Application);

void Application::Init()
{
    LUL::AppProperties::Get()->Init({ 
        L"An empty application", 
        L"An application" });

    L_LOG(LINFO, L"Create application");

    m_MainWindow->Init();
    m_SubWindow->Init();

};

void Application::Update()
{
}

void Application::Destroy()
{
    L_LOG(LINFO, L"Destroy application");
}
