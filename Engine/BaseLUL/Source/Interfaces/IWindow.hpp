#pragma once

#ifdef _WIN64

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

        bool _Create(PCWSTR windowName, LPCWSTR className)
        {
            WNDCLASSEX wcex = { 0 };

            wcex.cbSize = sizeof(WNDCLASSEX);
            wcex.style = CS_HREDRAW | CS_VREDRAW;
            wcex.lpfnWndProc = DERIVED_TYPE::WindowProc;
            wcex.hInstance = HINSTANCE();
            wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
            wcex.lpszClassName = className;

            RegisterClassEx(&wcex);

            m_Hwnd = CreateWindow(className,
                                  windowName,
                                  WS_OVERLAPPEDWINDOW,
                                  CW_USEDEFAULT,
                                  CW_USEDEFAULT,
                                  m_WindowSize.x,
                                  m_WindowSize.y,
                                  nullptr,
                                  nullptr,
                                  HINSTANCE(),
                                  this);

            return ( m_Hwnd ? true : false );
        }

    protected:

        HWND m_Hwnd = nullptr;

        LUL::Vec2<int32_t> m_WindowSize = { 1200, 800 };

    };

    class LUL_DLL IEmptyWindow : 
        public IBaseWindow<IEmptyWindow>,
        public LUL::IInitialable,
        public LUL::IUnknown
    {
    public:

        IEmptyWindow() = default;

        ~IEmptyWindow() = default;

    public:

        void Create(PCWSTR windowName, LPCWSTR className)
        {
            _Create(windowName, className);
        }

        void Show()
        {
            ShowWindow(m_Hwnd, SW_NORMAL);
        }

    public:

        // Getters

        const HWND GetHwnd() const { return m_Hwnd; }

        const LUL::Vec2<int32_t> GetWindowSize() const { return m_WindowSize; }

    };
}
#endif // _WIN64