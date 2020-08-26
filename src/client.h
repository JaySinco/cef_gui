#pragma once
#include <list>
#include "include/cef_client.h"

class MyClient: public CefClient,
                public CefDisplayHandler,
                public CefLifeSpanHandler
{
public:
    MyClient() {}

    // CefClient methods:
    CefRefPtr<CefDisplayHandler> GetDisplayHandler() override { return this; }
    CefRefPtr<CefLifeSpanHandler> GetLifeSpanHandler() override { return this; }

    // CefDisplayHandler methods:
    void OnTitleChange(CefRefPtr<CefBrowser> browser, const CefString& title) override;

    // CefLifeSpanHandler methods:
    void OnAfterCreated(CefRefPtr<CefBrowser> browser) override;
    bool DoClose(CefRefPtr<CefBrowser> browser) override;
    void OnBeforeClose(CefRefPtr<CefBrowser> browser) override;

private:
    std::list<CefRefPtr<CefBrowser>> browser_list_;
    IMPLEMENT_REFCOUNTING(MyClient);
};
