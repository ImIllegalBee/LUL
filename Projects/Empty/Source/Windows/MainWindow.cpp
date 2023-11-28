#include "MainWindow.hpp"

void MainWindow::Init()
{
    this->Create(std::wstring(LUL::GetApp()->GetAppCfg()->GetAppName()).c_str(),
                 std::wstring(LUL::GetApp()->GetAppCfg()->GetAppClass()).c_str());

    this->Show();
}

LRESULT MainWindow::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_DESTROY:
        {
            L_LOG(LINFO, L"Close rendering window");
            PostQuitMessage(0);
            return 0;
        }

        default:
        {
            return DefWindowProc(m_Hwnd, uMsg, wParam, lParam);
        }
    }
}
