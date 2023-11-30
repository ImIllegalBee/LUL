#pragma once

#define DECLARE_APPLICATION(appliactionClass) LUL::IApplication* LUL::EntryApplication() { return new appliactionClass; }

namespace LUL
{
    class LUL_DLL IApplication :
        public LUL::IInitialable,
        public LUL::IDestroyable
    {
    public:
        
        IApplication() = default;

        ~IApplication() = default;

    public:
        
        virtual void Update() = 0;

    };

    IApplication* EntryApplication();
}