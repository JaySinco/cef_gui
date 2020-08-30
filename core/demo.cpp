#include <windows.h>
#include <sstream>
#include "basic_app.h"

std::wstring calc_app_url()
{
    wchar_t szFilePath[MAX_PATH + 1] = {0};
    GetModuleFileNameW(NULL, szFilePath, MAX_PATH);
    (wcsrchr(szFilePath, L'\\'))[1] = 0;
    std::wostringstream ss;
    ss << L"file://" << szFilePath << L"resources/index.html";
    return ss.str();
}

int wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
    CefEnableHighDPISupport();
    CefMainArgs main_args(hInstance);
    CefRefPtr<BasicApp> app(new BasicApp(L"demo", 800, 600, calc_app_url()));
    int exit_code = CefExecuteProcess(main_args, app, nullptr);
    if (exit_code >= 0) {
        return exit_code;
    }
    CefSettings settings;
    settings.no_sandbox = true;
    settings.remote_debugging_port = 8081;
    CefInitialize(main_args, settings, app, nullptr);
    CefRunMessageLoop();
    CefShutdown();
    return 0;
}