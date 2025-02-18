#include "server.h"
#include "clipboard.h"
#include <windows.h>
#include <consoleapi.h>

#define WM_TRAYICON (WM_USER + 1)
#define WM_TIMER_UPDATE 1001 // Timer ID for periodic checks

LRESULT CALLBACK ClipProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    static SOCKET sockfd;
    static char *prevClipData = NULL;

    switch (uMsg) {
        case WM_CREATE:
            sockfd = socketCreate();
            SetTimer(hwnd, WM_TIMER_UPDATE, 1000, NULL); // Check clipboard every second
            return 0;

        case WM_DESTROY:
            KillTimer(hwnd, WM_TIMER_UPDATE);
            Shell_NotifyIcon(NIM_DELETE, NULL);
            free(prevClipData);
            close(sockfd);
            PostQuitMessage(0);
            return 0;

        case WM_COMMAND:
            if (LOWORD(wParam) == 1) {
                PostMessage(hwnd, WM_CLOSE, 0, 0);
            }
            return 0;

        case WM_TRAYICON:
            if (lParam == WM_RBUTTONDOWN) {
                POINT pt;
                GetCursorPos(&pt);
                HMENU hMenu = CreatePopupMenu();
                AppendMenu(hMenu, MF_STRING, 1, "Exit");

                SetForegroundWindow(hwnd);
                TrackPopupMenu(hMenu, TPM_BOTTOMALIGN | TPM_LEFTBUTTON, pt.x, pt.y, 0, hwnd, NULL);
                PostMessage(hwnd, WM_NULL, 0, 0);
                DestroyMenu(hMenu);
            }
            return 0;

        case WM_TIMER:
            if (wParam == WM_TIMER_UPDATE) {
                fd_set readfds;
                struct timeval timeout;
                size_t recBufferSize = 65507;
                char *recBuffer = malloc(recBufferSize);
                if (!recBuffer) {
                    perror("Memory allocation failed");
                    return 0;
                }

                bufferReceiver(sockfd, &readfds, &timeout, recBuffer, recBufferSize);

                char *currentClipData = clipboard();
                if (currentClipData && (!prevClipData || strcmp(currentClipData, prevClipData) != 0)) {
                    relay(sockfd, currentClipData);
                    free(prevClipData);
                    prevClipData = strdup(currentClipData);
                }
                free(currentClipData);
                free(recBuffer);
            }
            return 0;

        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
}

void createWind() {
    WNDCLASS wndClass = {0};
    wndClass.lpfnWndProc = ClipProc;
    wndClass.hInstance = GetModuleHandle(NULL);
    wndClass.lpszClassName = "ClipboardyTray";

    if (!RegisterClass(&wndClass)) {
        MessageBox(NULL, "Failed to register window class", "Error", MB_OK);
        return;
    }

    HWND hwnd = CreateWindowEx(0, wndClass.lpszClassName, "Clipboardy", 0, 
                               CW_USEDEFAULT, CW_USEDEFAULT, 0, 0, 
                               NULL, NULL, wndClass.hInstance, NULL);

    if (!hwnd) {
        MessageBox(NULL, "Failed to create window", "Error", MB_OK);
        return;
    }

    FreeConsole(); // Hide console window

    // Set up tray icon
    NOTIFYICONDATA nid = {0};
    nid.cbSize = sizeof(NOTIFYICONDATA);
    nid.hWnd = hwnd;
    nid.uID = 1;
    nid.uFlags = NIF_ICON | NIF_TIP | NIF_MESSAGE;
    nid.uCallbackMessage = WM_TRAYICON;
    nid.hIcon = (HICON)LoadImage(NULL, "icon.ico", IMAGE_ICON, 16, 16, LR_LOADFROMFILE);
    lstrcpy(nid.szTip, "Clipboardy Tray Application");

    if (!Shell_NotifyIcon(NIM_ADD, &nid)) {
        MessageBox(NULL, "Failed to add tray icon", "Error", MB_OK);
        return;
    }

    // Message loop
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    KillTimer(hwnd, WM_TIMER_UPDATE); // Cleanup
    Shell_NotifyIcon(NIM_DELETE, &nid);
}

int main() {
    createWind();
    return 0;
}
