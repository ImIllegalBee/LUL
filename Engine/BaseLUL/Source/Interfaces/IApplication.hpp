#pragma once

#define DECLARE_APPLICATION(appliactionClass) LUL_::IApplication* LUL_::EntryApplication() { return new appliactionClass; }

namespace LUL
{
    class LUL_DLL IApplication
    {
        
        friend LUL::AppProperties;
        friend LUL::Logger;

    public:
        
        IApplication(IN const wchar_t* appName,
                     IN const wchar_t* appClass) :
            m_pAppCfg(std::make_shared<AppProperties>(appName, appClass)),
            m_pLogg(std::make_shared<Logger>(m_pAppCfg))
        {
            m_pLogg->Log(LINFO, L"Create application");
        };

        ~IApplication()
        {
            m_pLogg->Log(LINFO, L"Destroy application");
            Sleep(1000);
        }

    public:
        
        virtual void Update() = 0;

    public:
        
        std::shared_ptr<const AppProperties> GetAppCfg() const { return m_pAppCfg; }

        std::shared_ptr<const Logger> GetLogger() const { return m_pLogg; }

    protected:
        
        const std::shared_ptr<AppProperties> m_pAppCfg = std::shared_ptr<AppProperties>(nullptr);
        const std::shared_ptr<Logger> m_pLogg = std::shared_ptr<Logger>(nullptr);
    };

    IApplication* EntryApplication();
}