// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include <iostream>
#include "irbis.h"
#include "rfid.h"

#ifdef IRBIS_WINDOWS

#include <windows.h>
#pragma comment(lib, "winscard.lib")

static const char CLASS_NAME[] = "SendChar Window Class";
static const char mainTitle[] = "SendChar by Miron";
static HWND mainWindow;
static std::string mainText ("........");
static const char readerName[] = "OMNIKEY CardMan 5x21-CL 0";
static irbis::WinSmartCards mgr;

static void sendText (HWND hwnd, const std::string &text)
{
    for (const auto c : text) {
        SendMessageA (hwnd, WM_CHAR, c, 0);
        Sleep (10);
    }
    SendMessageA (hwnd, WM_CHAR, '\r', 0);
    //SendMessageA (hwnd, WM_CHAR, '\n', 0);
}

void sendText (const std::string &text)
{
    HWND hwnd = GetForegroundWindow();
    DWORD threadId = GetWindowThreadProcessId (hwnd, nullptr);
    GUITHREADINFO gui;
    memset (&gui, 0, sizeof (gui));
    gui.cbSize = sizeof (gui);
    GetGUIThreadInfo(threadId, &gui);
    HWND hcaret = gui.hwndCaret;
    sendText (hcaret, text);
}

static void readCard()
{
    if (!mgr.connect(readerName)) {
        // PostQuitMessage(0);
        return;
    }

    const auto card = mgr.readCard();
    if (!card.empty()) {
        mainText = card;
        sendText (card);
        InvalidateRect (mainWindow, nullptr, true);
    }

    mgr.disconnect();
}

static void CALLBACK MyTimerProc (HWND Arg1, UINT Arg2, UINT_PTR Arg3, DWORD Arg4)
{
    readCard();
}

static LRESULT CALLBACK MyWindowProc (HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;

        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint (hwnd, &ps);

            FillRect (hdc, &ps.rcPaint, (HBRUSH) (COLOR_WINDOW + 1));
            DrawTextA (hdc, mainText.data(), static_cast<int>(mainText.size()), &ps.rcPaint,
                      DT_NOPREFIX|DT_CENTER|DT_VCENTER);

            EndPaint (hwnd, &ps);
        }
            return 0;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

static void registerClass (HINSTANCE hInstance)
{
    WNDCLASSA wc {};
    wc.lpfnWndProc   = MyWindowProc;
    wc.hInstance     = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hIcon         = LoadIconA (hInstance, "info");

    RegisterClassA (&wc);
}

static void createWindow (HINSTANCE hInstance)
{
    DWORD style = WS_CAPTION|WS_SYSMENU;
    DWORD styleEx = WS_EX_APPWINDOW;

    mainWindow = CreateWindowExA
            (
                    styleEx,      // Optional window styles.
                    CLASS_NAME,   // Window class
                    mainTitle,    // Window text
                    style,        // Window style

                    // Position
                    CW_USEDEFAULT, CW_USEDEFAULT,

                    // Size
                    250, 65,

                    nullptr,       // Parent window
                    nullptr,       // Menu
                    hInstance,     // Instance handle
                    nullptr        // Additional application data
            );

}

static void messageLoop()
{
    MSG msg {};
    while (GetMessage (&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE, PSTR pCmdLine, int nCmdShow)
{
    if (!irbis::isRfidSupported()) {
        MessageBoxA (nullptr, "RFID is not supported on this platform",
                    "ERROR", MB_OK|MB_ICONSTOP);
        return 1;
    }

    if (!mgr.open()) {
        MessageBoxA (nullptr, "Can't open RFID manager",
                    "ERROR", MB_OK|MB_ICONSTOP);
        return 1;
    }

    const auto readers = mgr.listReaders();
    if (readers.empty()) {
        MessageBoxA (nullptr, "No readers in the system",
                    "ERROR", MB_OK|MB_ICONSTOP);
        return 1;
    }

    registerClass (hInstance);
    createWindow (hInstance);
    ShowWindow (mainWindow, nCmdShow);
    SetTimer (mainWindow, 0, 1000, MyTimerProc);
    messageLoop();

    mgr.disconnect();
    mgr.close();

    return 0;
}

#else

int main()
{
    return 0;
}

#endif
