#pragma once
#include "include/cef_app.h"

class MyApp: public CefApp, public CefBrowserProcessHandler
{
public:
    MyApp() {}

    // CefApp methods:
    CefRefPtr<CefBrowserProcessHandler> GetBrowserProcessHandler() override { return this; }

    // CefBrowserProcessHandler methods:
    void OnContextInitialized() override;

private:
    IMPLEMENT_REFCOUNTING(MyApp);
};