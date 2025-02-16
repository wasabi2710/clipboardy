#include "server.h"
#include "clipboard.h"
#include <windows.h>

#define WM_TRAYICON (WM_APP + 1)
#define WM_TIMER_UPDATE 1001 // Timer identifier

LRESULT CALLBACK ClipProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    static SOCKET sockfd;
    static char *prevClipData = NULL;

    switch (uMsg) {
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        case WM_COMMAND:
            if (LOWORD(wParam) == 1 || LOWORD(wParam) == 2) {
                PostQuitMessage(0);
            }
            return 0;
        case WM_USER + 1:
            if (lParam == WM_RBUTTONDOWN) {
                POINT pt;
                GetCursorPos(&pt);
                HMENU hMenu = CreatePopupMenu();
                AppendMenu(hMenu, MF_STRING, 1, "Exit");
                AppendMenu(hMenu, MF_STRING, 2, "Close");

                SetForegroundWindow(hwnd);
                TrackPopupMenu(hMenu, TPM_BOTTOMALIGN | TPM_LEFTBUTTON, pt.x, pt.y, 0, hwnd, NULL);
                PostMessage(hwnd, WM_NULL, 0, 0);

                DestroyMenu(hMenu);
            }
            return 0;
        case WM_CREATE:
            sockfd = socketCreate();
            return 0;
        case WM_TIMER:
            if (wParam == WM_TIMER_UPDATE) {
                fd_set readfds;
                struct timeval timeout;
                size_t recBufferSize = 65507;
                char* recBuffer = malloc(recBufferSize);

                if (!recBuffer) {
                    perror("Memory allocation failed");
                    return EXIT_FAILURE;
                }

                bufferReceiver(sockfd, readfds, timeout, recBuffer, recBufferSize);

                char* currentClipData = clipboard();
                if (currentClipData && (!prevClipData || strcmp(currentClipData, prevClipData) != 0)) {
                    relay(sockfd, currentClipData);
                    free(prevClipData);
                    prevClipData = currentClipData;
                } else {
                    free(currentClipData);
                }

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

    HWND window = CreateWindowEx(
        0,
        wndClass.lpszClassName,
        "Clipboardy",
        0,
        CW_USEDEFAULT, CW_USEDEFAULT,
        0, 0,
        NULL, NULL,
        wndClass.hInstance,
        NULL
    );

    if (!window) {
        MessageBox(NULL, "Failed to create window", "Error", MB_OK);
        return;
    }

    NOTIFYICONDATA nid = {0};
    nid.cbSize = sizeof(NOTIFYICONDATA);
    nid.hWnd = window;
    nid.uID = 1;
    nid.uFlags = NIF_ICON | NIF_TIP | NIF_MESSAGE;
    nid.uCallbackMessage = WM_USER + 1;
    nid.hIcon = LoadIcon(NULL, IDI_INFORMATION);
    lstrcpy(nid.szTip, "Clipboardy Tray Application");

    if (!Shell_NotifyIcon(NIM_ADD, &nid)) {
        MessageBox(NULL, "Failed to add tray icon", "Error", MB_OK);
        return;
    }

    SetTimer(window, WM_TIMER_UPDATE, 1000, NULL); // Set timer for periodic updates

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        if (msg.message == WM_COMMAND && (msg.wParam == 1 || msg.wParam == 2)) {
            Shell_NotifyIcon(NIM_DELETE, &nid);
            PostQuitMessage(0);
        }

        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    KillTimer(window, WM_TIMER_UPDATE); // Clean up timer
    Shell_NotifyIcon(NIM_DELETE, &nid);
}

int main() {
    createWind();
    return 0;
}
