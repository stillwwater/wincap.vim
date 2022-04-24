/*
 * wincap.c
 *
 * Usage wincap.exe [PROCESS_NAME] [COLOR]
 *   ex. wincap.exe gvim.exe 0C0C0C
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <windows.h>
#include <dwmapi.h>
#include <tlhelp32.h>

struct enum_windows_params {
    DWORD pid;
    HWND hwnd;
};

struct wincap_config {
    const char *process_name;
    uint32_t color;
};

enum DWMWINDOWATTRIBUTE_ {
    DWMWA_USE_IMMERSIVE_DARK_MODE_ = 20,
    DWMWA_WINDOW_CORNER_PREFERENCE_ = 33,
    DWMWA_BORDER_COLOR_,
    DWMWA_CAPTION_COLOR_,
    DWMWA_TEXT_COLOR_,
    DWMWA_VISIBLE_FRAME_BORDER_THICKNESS_,
    DWMWA_LAST_,
};

struct wincap_config parse_args(int argc, char *argv[])
{
    struct wincap_config conf;
    conf.process_name = argc > 1 ? argv[1] : "gvim.exe";
    conf.color = argc > 2 ? strtol(argv[2], NULL, 16) : 0;

    return conf;
}

BOOL CALLBACK enum_windows(HWND hwnd, LPARAM lparam)
{
    struct enum_windows_params *params = (struct enum_windows_params *)lparam;
    DWORD pid = 0;
    GetWindowThreadProcessId(hwnd, &pid);

    if (params->pid != pid || GetWindow(hwnd, GW_OWNER) != NULL) {
        return TRUE;
    }

    params->hwnd = hwnd;
    return FALSE;
}

HWND find_window(DWORD pid)
{
    struct enum_windows_params result;
    result.pid = pid;
    result.hwnd = NULL;

    EnumWindows(enum_windows, (LPARAM)&result);
    return result.hwnd;
}

int main(int argc, char *argv[])
{
    struct wincap_config config = parse_args(argc, argv);
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    PROCESSENTRY32 process;

    ZeroMemory(&process, sizeof(process));
    process.dwSize = sizeof(process);

    if (Process32First(snapshot, &process)) {
        do {
            if (!strcmp(process.szExeFile, config.process_name)) {
                COLORREF color = (COLORREF)config.color;
                HWND hwnd = find_window(process.th32ProcessID);

                DwmSetWindowAttribute(hwnd, DWMWA_CAPTION_COLOR_, &color, sizeof(color));
            }
        } while (Process32Next(snapshot, &process));
    }

    return 0;
}
