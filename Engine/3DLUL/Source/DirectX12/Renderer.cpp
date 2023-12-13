#include "3DLUL.h"

#include "Vertex.h"

USING_DX12();

void LogDX12(D3D12_MESSAGE_CATEGORY Category,
             D3D12_MESSAGE_SEVERITY Severity,
             D3D12_MESSAGE_ID ID,
             LPCSTR pDescription,
             void* pContext)
{
    LUL::LogTags logType = {};
    std::string categoryStr = "";

    switch (Severity)
    {
        case ( D3D12_MESSAGE_SEVERITY_CORRUPTION ):
        {
            logType = LCORR;
            break;
        }
        case ( D3D12_MESSAGE_SEVERITY_ERROR ):
        {
            logType = LERR;
            break;
        }
        case ( D3D12_MESSAGE_SEVERITY_WARNING ):
        {
            logType = LWARN;
            break;
        }
        case ( D3D12_MESSAGE_SEVERITY_INFO ):
        {
            logType = LINFO;
            break;
        }
        case ( D3D12_MESSAGE_SEVERITY_MESSAGE ):
        {
            logType = LMESS;
            break;
        }
        default:
        {
            logType = LPLAIN;
            break;
        }
    }

    switch (Category)
    {
        case ( D3D12_MESSAGE_CATEGORY_APPLICATION_DEFINED ):
        {
            categoryStr = "APPLICATION_DEFINED";
            break;
        }
        case ( D3D12_MESSAGE_CATEGORY_MISCELLANEOUS ):
        {
            categoryStr = "MISCELLANEOUS";
            break;
        }
        case ( D3D12_MESSAGE_CATEGORY_INITIALIZATION ):
        {
            categoryStr = "INITIALIZATION";
            break;
        }
        case ( D3D12_MESSAGE_CATEGORY_CLEANUP ):
        {
            categoryStr = "CLEANUP";
            break;
        }
        case ( D3D12_MESSAGE_CATEGORY_COMPILATION ):
        {
            categoryStr = "COMPILATION";
            break;
        }
        case ( D3D12_MESSAGE_CATEGORY_STATE_CREATION ):
        {
            categoryStr = "STATE_CREATION";
            break;
        }
        case ( D3D12_MESSAGE_CATEGORY_STATE_SETTING ):
        {
            categoryStr = "STATE_SETTING";
            break;
        }
        case ( D3D12_MESSAGE_CATEGORY_STATE_GETTING ):
        {
            categoryStr = "STATE_GETTING";
            break;
        }
        case ( D3D12_MESSAGE_CATEGORY_RESOURCE_MANIPULATION ):
        {
            categoryStr = "RESOURCE_MANIPULATION";
            break;
        }
        case ( D3D12_MESSAGE_CATEGORY_EXECUTION ):
        {
            categoryStr = "EXECUTION";
            break;
        }
        case ( D3D12_MESSAGE_CATEGORY_SHADER ):
        {
            categoryStr = "SHADER";
            break;
        }
        default:
        {
            categoryStr = "UNKNOWN";
            break;
        }
    }

    LUL::ApiAddToLogQueue(logType,
                          L"[ DX12 ][ %S ][ ID = %d ] %S",
                          categoryStr.c_str(),
                          ID,
                          pDescription);
}


LUL::Win64::DXRenderer::DXRenderer(HWND hwndRenderTarget) :
    m_HWNDRenderTarget(hwndRenderTarget)
{}

LUL::Win64::DXRenderer::~DXRenderer()
{}

void LUL::Win64::DXRenderer::Init()
{
    LoadPipeline();
    LoadAssets();
}

void LUL::Win64::DXRenderer::Update()
{}

void LUL::Win64::DXRenderer::Render()
{}

void LUL::Win64::DXRenderer::Destroy()
{
    m_Hardware->~Hardware();

    ReportLiveObjects();
}

// PRIVATE --------------------------------------------------------------------

void LUL::Win64::DXRenderer::LoadPipeline()
{
    uint32_t dxgiFactoryFlags = 0;

    #ifdef _DEBUG
    {
        ComPtr<ID3D12Debug> debugController = 0;
        if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
        {
            debugController->EnableDebugLayer();

            dxgiFactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
        }
    }
    #endif // _DEBUG

    ComPtr<IDXGIFactory> factory = 0;
    Excpt64::ThrowIfFailed(CreateDXGIFactory(IID_PPV_ARGS(&factory)));

    ComPtr<IDXGIFactory2> factory2 = 0;
    if (SUCCEEDED(factory->QueryInterface(IID_PPV_ARGS(&factory2))))
    {
        Excpt64::ThrowIfFailed(CreateDXGIFactory2(dxgiFactoryFlags, IID_PPV_ARGS(&factory2)));

        if (factory2.Get())
        {
            factory.Swap(factory2);
            factory2.~ComPtr();
        }
    }

    m_Hardware->Initialize(factory);

    #ifdef _DEBUG
    {
        ComPtr<ID3D12InfoQueue1> infoQueue = 0;
        m_Hardware->GetDevice()->QueryInterface(IID_PPV_ARGS(&infoQueue));
        DWORD cookie = 0;
        if (infoQueue)
            infoQueue->RegisterMessageCallback(LogDX12,
                                               D3D12_MESSAGE_CALLBACK_FLAG_NONE,
                                               nullptr,
                                               &cookie);
        if (!cookie)
            L_LOG(LERR, L"Couldn't start DX12 debug message callback.");
    }
    #endif // _DEBUG
}

void LUL::Win64::DXRenderer::LoadAssets()
{}

void LUL::Win64::DXRenderer::ReportLiveObjects()
{
    L_LOG(LINFO, L"ReportLiveObjects \\/\\/\\/\\/\\/\\/\\/\\/\\/ ");

    Microsoft::WRL::ComPtr<IDXGIDebug1> dxgiDebug = 0;
    if (SUCCEEDED(DXGIGetDebugInterface1(0, IID_PPV_ARGS(dxgiDebug.GetAddressOf()))))
    {
        dxgiDebug->ReportLiveObjects(DXGI_DEBUG_ALL, DXGI_DEBUG_RLO_FLAGS(DXGI_DEBUG_RLO_DETAIL | DXGI_DEBUG_RLO_IGNORE_INTERNAL));
    }

    L_LOG(LINFO, L"ReportLiveObjects ^^^^^^^^^^ ");
}
