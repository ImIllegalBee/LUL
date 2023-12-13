#include "3DLUL.h"

USING_DX12();

void LUL::Win64::Hardware::Initialize(const Microsoft::WRL::ComPtr<IDXGIFactory>& factory)
{
    ComPtr<IDXGIAdapter1> adapter1 = 0;
    ComPtr<IDXGIFactory6> factory6 = 0;

    if (SUCCEEDED(factory->QueryInterface(IID_PPV_ARGS(&factory6))))
    {
        DXGI_ADAPTER_DESC1 desc1 = {};

        for (uint32_t adtprI = 0;
             SUCCEEDED(factory6->EnumAdapterByGpuPreference(adtprI,
             DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE,
             IID_PPV_ARGS(&adapter1)));
             adtprI++)
        {
            adapter1->GetDesc1(&desc1);

            if (desc1.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
                continue;

            if (SUCCEEDED(D3D12CreateDevice(adapter1.Get(),
                D3D_FEATURE_LEVEL_12_0,
                _uuidof(ID3D12Device),
                nullptr)))
            {
                break;
            }
        }
    }

    if (!adapter1.Get())
    {
        ComPtr<IDXGIFactory1> factory1 = 0;
        Excpt64::ThrowIfFailed(factory->QueryInterface(IID_PPV_ARGS(&factory1)));

        DXGI_ADAPTER_DESC1 desc1 = {};

        for (uint32_t adtprI = 0;
             SUCCEEDED(factory1->EnumAdapters1(adtprI,
             &adapter1));
             adtprI++)
        {
            adapter1->GetDesc1(&desc1);

            if (desc1.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
                continue;

            if (SUCCEEDED(D3D12CreateDevice(adapter1.Get(),
                D3D_FEATURE_LEVEL_12_0,
                _uuidof(ID3D12Device),
                nullptr)))
            {
                break;
            }
        }
    }

    m_Adapter = adapter1.Detach();

    if (!m_Adapter.Get())
        LUL::Except::Internal(-1);

    Excpt64::ThrowIfFailed(D3D12CreateDevice(
        m_Adapter.Get(),
        D3D_FEATURE_LEVEL_12_0,
        IID_PPV_ARGS(&m_GPU)));
}
