#pragma once

#ifdef _WIN64

#pragma warning ( push )
// Warning C4251 class needs to have dll - interface to be used by clients of class 
#pragma warning ( disable : 4251)

namespace LUL::Win64
{
    template<class DERIVED_TYPE>
    class LUL_DLL IBaseWindow
    {
    public:

        IBaseWindow() = default;

        ~IBaseWindow() = default;

    public:

        static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
        {
            DERIVED_TYPE* pThis = NULL;

            if (uMsg == WM_NCCREATE)
            {
                CREATESTRUCT* pCreate = (CREATESTRUCT*) lParam;
                pThis = (DERIVED_TYPE*) pCreate->lpCreateParams;
                SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR) pThis);

                pThis->m_Hwnd = hwnd;
            }
            else
            {
                pThis = (DERIVED_TYPE*) GetWindowLongPtr(hwnd, GWLP_USERDATA);
            }
            if (pThis)
            {
                return pThis->HandleMessage(uMsg, wParam, lParam);
            }
            else
            {
                return DefWindowProc(hwnd, uMsg, wParam, lParam);
            }
        }

    protected:

        virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) = 0;

        bool _Create()
        {
            WNDCLASSEX wcex = { 0 };

            wcex.cbSize = sizeof(WNDCLASSEX);
            wcex.style = CS_HREDRAW | CS_VREDRAW;
            wcex.lpfnWndProc = DERIVED_TYPE::WindowProc;
            wcex.hInstance = HInstance();
            wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
            wcex.lpszClassName = m_WindowClass.c_str();

            RegisterClassEx(&wcex);

            m_Hwnd = CreateWindow(m_WindowClass.c_str(),
                                  m_WindowName.c_str(),
                                  WS_OVERLAPPEDWINDOW,
                                  CW_USEDEFAULT,
                                  CW_USEDEFAULT,
                                  m_WindowSize.x,
                                  m_WindowSize.y,
                                  nullptr,
                                  nullptr,
                                  HInstance(),
                                  this);

            return ( m_Hwnd ? true : false );
        }

    protected:

        HWND m_Hwnd = nullptr;

        LUL::Vec2<int32_t> m_WindowSize = { 1200, 800 };

        std::wstring m_WindowName = L"";
        std::wstring m_WindowClass = L"";
    };

    class LUL_DLL IEmptyWindow : 
        public IBaseWindow<IEmptyWindow>,
        public LUL::IInitialable,
        public LUL::IDestroyable,
        public LUL::IUnknown
    {
    public:

        IEmptyWindow() = default;

        ~IEmptyWindow() = default;

    public:

        void Create(PCWSTR windowName, LPCWSTR className)
        {
            m_WindowName = windowName;
            m_WindowClass = className;

            LUL::ApiAddToLogQueue(LINFO, L"Creating window '%ls'", m_WindowName.c_str());

            _Create();
        }

        void Show()
        {
            LUL::ApiAddToLogQueue(LINFO, L"Showing window '%ls'", m_WindowName.c_str());

            ShowWindow(m_Hwnd, SW_NORMAL);
        }

        void Destroy() override
        {
            LUL::ApiAddToLogQueue(LINFO, L"Destroying window '%ls'", m_WindowName.c_str());
            CloseWindow(m_Hwnd);
        }

    public:

        // Getters

        const HWND GetHwnd() const { return m_Hwnd; }

        const LUL::Vec2<int32_t> GetWindowSize() const { return m_WindowSize; }

    };
}

#pragma warning ( pop )

#endif // _WIN64