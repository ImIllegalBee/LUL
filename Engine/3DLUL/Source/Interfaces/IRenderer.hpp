#pragma once

namespace LUL
{
    class LUL_DLL IRenderer : 
        public LUL::IUnknown,
        public LUL::IInitialable,
        public LUL::IDestroyable
    {
    public:

        IRenderer() = default;

        ~IRenderer() = default;

    public:

        virtual void Update() = 0;

        virtual void Render() = 0;

    };
}