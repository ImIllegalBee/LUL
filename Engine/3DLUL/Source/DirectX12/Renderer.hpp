#pragma once


#ifdef _WIN64

namespace LUL::Win64
{

    class LUL_DLL DXRenderer :
        public LUL::IRenderer
    {
    public:

        DXRenderer();

        ~DXRenderer();

    public:

        virtual void Init() override;

        virtual void Update() override;

        virtual void Render() override;

        virtual void Destroy() override;

    private:

        HWND m_HWNDRenderTarget = nullptr;

    };

}

#endif // _WIN64
