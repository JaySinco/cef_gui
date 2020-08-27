#include <windows.h>
#include "app.h"

int wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
    CefEnableHighDPISupport();
    CefMainArgs main_args(hInstance);
    int exit_code = CefExecuteProcess(main_args, nullptr, nullptr);
    if (exit_code >= 0) {
        return exit_code;
    }
    CefRefPtr<MyApp> app(new MyApp);
    CefSettings settings;
    settings.no_sandbox = true;
    settings.remote_debugging_port = 8081;
    CefInitialize(main_args, settings, app, nullptr);
    CefRunMessageLoop();
    CefShutdown();
    return 0;
}