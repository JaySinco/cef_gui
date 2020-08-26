#include "include/cef_browser.h"
#include "include/cef_command_line.h"
#include "include/views/cef_browser_view.h"
#include "include/views/cef_window.h"
#include "include/wrapper/cef_helpers.h"
#include "app.h"
#include "client.h"

namespace
{

class MyWindowDelegate : public CefWindowDelegate
{
public:
    explicit MyWindowDelegate(CefRefPtr<CefBrowserView> browser_view)
      : browser_view_(browser_view) {}

    void OnWindowCreated(CefRefPtr<CefWindow> window) override
    {
        window->AddChildView(browser_view_);
        window->Show();
        browser_view_->RequestFocus();
    }

    void OnWindowDestroyed(CefRefPtr<CefWindow> window) override
    {
        browser_view_ = nullptr;
    }

    bool CanClose(CefRefPtr<CefWindow> window) override
    {
        CefRefPtr<CefBrowser> browser = browser_view_->GetBrowser();
        if (browser) {
            return browser->GetHost()->TryCloseBrowser();
        }
        return true;
    }

    CefSize GetPreferredSize(CefRefPtr<CefView> view)
    {
        return CefSize(800, 600);
    }

private:
    CefRefPtr<CefBrowserView> browser_view_;

    IMPLEMENT_REFCOUNTING(MyWindowDelegate);
    DISALLOW_COPY_AND_ASSIGN(MyWindowDelegate);
};

class MyBrowserViewDelegate : public CefBrowserViewDelegate
{
public:
    MyBrowserViewDelegate() {}

    bool OnPopupBrowserViewCreated(
        CefRefPtr<CefBrowserView> browser_view,
        CefRefPtr<CefBrowserView> popup_browser_view,
        bool is_devtools) override
    {
        CefWindow::CreateTopLevelWindow(new MyWindowDelegate(popup_browser_view));
        return true;
    }

private:
    IMPLEMENT_REFCOUNTING(MyBrowserViewDelegate);
    DISALLOW_COPY_AND_ASSIGN(MyBrowserViewDelegate);
};

}

void MyApp::OnContextInitialized()
{
    CEF_REQUIRE_UI_THREAD();
    CefRefPtr<MyClient> handler(new MyClient);
    auto command_line = CefCommandLine::GetGlobalCommandLine();
    std::string url = command_line->GetSwitchValue("url");
    if (url.empty()) {
        url = "https://www.baidu.com";
    }
    CefBrowserSettings browser_settings;
    auto browser_view = CefBrowserView::CreateBrowserView(
        handler, url, browser_settings, nullptr, nullptr,
        new MyBrowserViewDelegate());
    CefWindow::CreateTopLevelWindow(new MyWindowDelegate(browser_view));
}

