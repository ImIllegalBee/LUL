#include "MainWindow.hpp"

void MainWindow::Init()
{
    Create(LUL::AppProperties::Get()->GetAppName().c_str(),
           LUL::AppProperties::Get()->GetAppClass().c_str());

    Show();
}

LRESULT MainWindow::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_DESTROY:
        {
            L_LOG(LINFO, L"Close MainWindow");
            PostQuitMessage(0);
            return 0;
        }

        default:
        {
            return DefWindowProc(m_Hwnd, uMsg, wParam, lParam);
        }
    }
}
