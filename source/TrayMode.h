// TrayMode.h: interface for the CTrayMode class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#define WM_TRAY_MODE_ICON (WM_USER+100)
#define WM_TRAY_MODE_MESSAGE (WM_USER+101)

class CTrayMode
{
public:
    CTrayMode();
    void Init(HINSTANCE hins);
    void Toggle();
#ifdef _WIN64
    LONG_PTR GetMainWndProc();  // 64 bits
#else
    LONG GetMainWndProc();      // 32 bits
#endif
    void ShowNotify(bool mode);
    static LRESULT CALLBACK TrayModeWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
    HICON m_TrayIcon;
private:
    WNDPROC m_MainWndProc;
};


extern CTrayMode gTrayMode;
