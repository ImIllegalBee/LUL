#pragma once

#include "BaseLUL.h"

#ifdef _DX12

#include <wrl.h>

#include <d3d12.h>
#include <dxgi1_6.h>
#include <d2d1.h>
#include <dwrite.h>
#include <D3Dcompiler.h>
#include <DirectXMath.h>
#include <d3d11on12.h>
#include <dxgidebug.h>

#pragma comment(lib, "d2d1")
#pragma comment(lib, "dxgi")
#pragma comment(lib, "D3D11")
#pragma comment(lib, "d3d12")
#pragma comment(lib, "D3Dcompiler")
#pragma comment(lib, "Dwrite")
#pragma comment(lib, "dxguid")
#pragma comment(lib, "kernel32")

#ifndef USING_DX12
#define USING_DX12()                    \
using namespace DirectX;                \
using Microsoft::WRL::ComPtr;           \
using Microsoft::WRL::WeakRef;          \
namespace Excpt64 = LUL::Except::Win64; 
#endif // !USING_DX12

#endif // _DX12

#pragma region Compiler warnings suppression
#pragma warning ( push )
// Warning C4251 class needs to have dll - interface to be used by clients of class 
#pragma warning ( disable : 4251)
#pragma endregion

#pragma region Engine includes

    #include "Interfaces/IRenderer.hpp"

#ifdef _DX12
    #include "DirectX12/Renderer.hpp"
#endif // _DX12

#pragma endregion

#pragma warning ( pop )