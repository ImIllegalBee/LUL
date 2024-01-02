#include "Application.hpp"

DECLARE_APPLICATION(Application);

void Application::Init()
{
    LUL_INITIALIZE_CORE_LIST({ 
        L"An empty application", 
        L"An application" });

    L_LOG(LINFO, L"Create application");

    m_MainWindow->Init();
   
    m_Renderer = std::make_shared<LUL::Win64::DXRenderer>(m_MainWindow->GetHwnd());
    m_Renderer->Init();

    m_SubWindow->Init();
};

void Application::Update()
{
    m_Renderer->Update();
    m_Renderer->Render();
}

void Application::Destroy()
{
    L_LOG(LINFO, L"Destroy application");

    m_MainWindow->Destroy();
    m_SubWindow->Destroy();

    m_Renderer->Destroy();
}
