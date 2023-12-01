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

    };
}

#endif // _WIN64
