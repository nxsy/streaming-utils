#include <windows.h>

struct
{
    const char *title_match;
    int width;
    int height;
    int left;
    int top;
    bool client_adjust;
} matches[] =
{
    {"GVIM", 1928, 1111, 0, 0, true},
    {"Microsoft Visual Studio", 1920, 1080, 0, 0, false},
    {"Open Broadcaster Software", 640, 640, 1920, 0, false},
};

int CALLBACK
WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    HWND hwnd = 0;
    while ((hwnd = FindWindowEx(0, hwnd, 0, 0)) != 0)
    {
        char buffer[100];
        GetWindowText(hwnd, buffer, 100);

        WINDOWINFO info = {};
        info.cbSize = sizeof(info);
        GetWindowInfo(hwnd, &info);

        BOOL enabled = IsWindowEnabled(hwnd);
        BOOL visible = IsWindowVisible(hwnd);
        DWORD is_popup = info.dwStyle & WS_POPUP;

        if (enabled && visible && !is_popup)
        {
            for (auto &&m : matches)
            {
                if (strstr(buffer, m.title_match))
                {
                    RECT rect = {};
                    rect.left = m.left;
                    rect.top = m.top;
                    rect.right = m.width;
                    rect.bottom = m.height;
                    if (m.client_adjust)
                    {
                        AdjustWindowRectEx(&rect, info.dwStyle, 0, info.dwExStyle);
                    }
                    SetWindowPos(hwnd, 0, rect.left, rect.top, rect.right, rect.bottom, SWP_NOREPOSITION | SWP_NOZORDER);
                }
            }
        }
    }
    return 0;
}
