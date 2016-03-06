#include <stdint.h>
#include <stdio.h>
#include <windows.h>

struct
{
    const char *title_match;
    int32_t width;
    int32_t height;
    int32_t left;
    int32_t top;
    bool client_adjust;

} matches[] = {
    {"GVIM", 1928, 1111, 0, 0, true},
    {"Microsoft Visual Studio", 1920, 1080, 0, 0, false},
    {"Open Broadcaster Software", 640, 640, 1920, 0, false},
};

void
handle_hwnd(HWND hwnd)
{
    char buffer[100];
    char ods_buffer[100];
    GetWindowText(hwnd, buffer, 100);

    WINDOWINFO info = {};
    info.cbSize = sizeof(info);

    GetWindowInfo(hwnd, &info);


    BOOL enabled = IsWindowEnabled(hwnd);
    BOOL visible = IsWindowVisible(hwnd);
    DWORD is_child = info.dwStyle & WS_CHILD;
    DWORD is_popup = info.dwStyle & WS_POPUP;

    const char *enabled_text = "Enabled";
    const char *visible_text = "Visible";
    const char *child_text = "!child";
    const char *popup_text = "!popup";

    if (!enabled)
    {
        enabled_text = "Disabled";
    }

    if (!visible)
    {
        visible_text = "Invisible";
    }

    if (is_child)
    {
        child_text = "child";
    }

    if (is_popup)
    {
        popup_text = "popup";
    }

    if (enabled && visible && !is_popup)
    {
        _snprintf(ods_buffer, 100, "%s - %s, %s\n", buffer, child_text, popup_text);
        OutputDebugString(ods_buffer);

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

BOOL CALLBACK
enumerate_windows(HWND hwnd, LPARAM lParam)
{
    handle_hwnd(hwnd);
    return true;
}

int CALLBACK
WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    //EnumWindows(enumerate_windows, 0);

    HWND child_after = 0;
    while ((child_after = FindWindowEx(0, child_after, 0, 0)) != 0)
    {
        handle_hwnd(child_after);
    }
    return 0;
}
