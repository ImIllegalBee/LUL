#include "SubWindow.hpp"

void SubWindow::Init()
{
    this->m_WindowSize = { 600, 400 };

    // Create(L"SubWindow",
    //        LUL::GetApp()->GetAppCfg()->GetAppClass().c_str());

    Show();
}

LRESULT SubWindow::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        default:
        {
            return DefWindowProc(m_Hwnd, uMsg, wParam, lParam);
        }
    }
}
