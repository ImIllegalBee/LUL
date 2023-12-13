#pragma once

#ifdef _WIN64

#include "Hardware.hpp"

namespace LUL::Win64
{
    class LUL_DLL DXRenderer :
        public LUL::IRenderer
    {
    public:

        DXRenderer(HWND hwndRenderTarget);

        ~DXRenderer();

    public:

        virtual void Init() override;

        virtual void Update() override;

        virtual void Render() override;

        virtual void Destroy() override;

    private:

        void LoadPipeline();

        void LoadAssets();

        static void ReportLiveObjects();

    private:

        HWND m_HWNDRenderTarget = nullptr;

    private:

        // D3D12

        std::shared_ptr<LUL::Win64::Hardware> m_Hardware = std::make_shared<LUL::Win64::Hardware>();

    };

}

#endif // _WIN64
