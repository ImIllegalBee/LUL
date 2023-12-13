#pragma once

#ifdef _WIN64

namespace LUL::Win64
{
    class LUL_DLL Hardware
    {
    public:

        Hardware() = default;

        ~Hardware() = default;

    public:

        void Initialize(const Microsoft::WRL::ComPtr<IDXGIFactory>& factory);

    public:

        // Getters

        Microsoft::WRL::ComPtr<ID3D12Device> GetDevice() { return m_GPU; }

    private:

        Microsoft::WRL::ComPtr<IDXGIAdapter> m_Adapter = 0;

        Microsoft::WRL::ComPtr<ID3D12Device> m_GPU = 0;

    };
}

#endif // _WIN64
