#include "include/cef_browser.h"
#include "include/cef_command_line.h"
#include "include/views/cef_browser_view.h"
#include "include/views/cef_window.h"
#include "include/wrapper/cef_helpers.h"
#include "app.h"
#include "client.h"

void MyApp::OnContextInitialized()
{
    CEF_REQUIRE_UI_THREAD();
    auto command_line = CefCommandLine::GetGlobalCommandLine();
    std::string url = command_line->GetSwitchValue("url");
    if (url.empty()) {
        url = "https://www.baidu.com";
    }
    CefRefPtr<MyClient> handler(new MyClient);
    CefBrowserSettings browser_settings;
    CefWindowInfo window_info;
    window_info.SetAsPopup(NULL, "mygui");
    window_info.width = 1600;
    window_info.height = 900;
    CefBrowserHost::CreateBrowser(window_info, handler, url, browser_settings, nullptr, nullptr);
}

