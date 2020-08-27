#include <windows.h>
#include "include/base/cef_bind.h"
#include "include/cef_app.h"
#include "include/cef_parser.h"
#include "include/views/cef_browser_view.h"
#include "include/views/cef_window.h"
#include "include/wrapper/cef_closure_task.h"
#include "include/wrapper/cef_helpers.h"
#include "client.h"

namespace
{

std::string GetDataURI(const std::string& data, const std::string& mime_type) {
    return "data:" + mime_type + ";base64," + CefURIEncode(
        CefBase64Encode(data.data(), data.size()), false).ToString();
}

}

void MyClient::OnTitleChange(CefRefPtr<CefBrowser> browser, const CefString& title)
{
    CEF_REQUIRE_UI_THREAD();
    CefWindowHandle hwnd = browser->GetHost()->GetWindowHandle();
    SetWindowTextW(hwnd, std::wstring(title).c_str());
}

void MyClient::OnAfterCreated(CefRefPtr<CefBrowser> browser)
{
    CEF_REQUIRE_UI_THREAD();
    browser_list_.push_back(browser);
}

bool MyClient::DoClose(CefRefPtr<CefBrowser> browser)
{
    CEF_REQUIRE_UI_THREAD();
    return false;
}

void MyClient::OnBeforeClose(CefRefPtr<CefBrowser> browser)
{
    CEF_REQUIRE_UI_THREAD();
    for (auto it = browser_list_.begin(); it != browser_list_.end(); ++it) {
        if ((*it)->IsSame(browser)) {
            browser_list_.erase(it);
            break;
        }
    }
    if (browser_list_.empty()) {
        CefQuitMessageLoop();
    }
}

bool MyClient::OnBeforePopup(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, const CefString& target_url,
    const CefString& target_frame_name, CefLifeSpanHandler::WindowOpenDisposition target_disposition, bool user_gesture,
    const CefPopupFeatures& popupFeatures, CefWindowInfo& windowInfo, CefRefPtr<CefClient>& client,
    CefBrowserSettings& settings, CefRefPtr<CefDictionaryValue>& extra_info, bool* no_javascript_access)
{
    CEF_REQUIRE_UI_THREAD();
    browser->GetMainFrame()->LoadURL(target_url);
    return true;
}

bool MyClient::OnOpenURLFromTab(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, const CefString& target_url,
    CefRequestHandler::WindowOpenDisposition target_disposition, bool user_gesture)
{
    CEF_REQUIRE_UI_THREAD();
    return false;
}

bool MyClient::OnKeyEvent(CefRefPtr<CefBrowser> browser, const CefKeyEvent& event, CefEventHandle os_event)
{
    CEF_REQUIRE_UI_THREAD();
    if (event.type == KEYEVENT_RAWKEYDOWN) {
        if (event.windows_key_code == VK_F5) {
            browser->Reload();
            return true;
        }
        if (event.windows_key_code == VK_F12) {
            if (browser->GetHost()->HasDevTools()) {
                browser->GetHost()->CloseDevTools();
            }
            else {
                CefBrowserSettings browser_settings;
                CefWindowInfo window_info;
                window_info.SetAsPopup(browser->GetHost()->GetWindowHandle(), "DevTools");
                window_info.width = 1200;
                window_info.height = 675;
                CefPoint pt(0, 0);
                browser->GetHost()->ShowDevTools(window_info, nullptr, browser_settings, pt);
            }
            return true;
        }
        if (event.windows_key_code == VK_LEFT && (event.modifiers & EVENTFLAG_ALT_DOWN)) {
            browser->GoBack();
            return true;
        }
        if (event.windows_key_code == VK_RIGHT && (event.modifiers & EVENTFLAG_ALT_DOWN)) {
            browser->GoForward();
            return true;
        }
    }
    return false;
}

void MyClient::OnLoadError(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, ErrorCode errorCode,
    const CefString& errorText, const CefString& failedUrl)
{
    CEF_REQUIRE_UI_THREAD();
    if (errorCode == ERR_ABORTED)
        return;
    std::stringstream ss;
    ss << "<html><body bgcolor=\"white\"><h2>Failed to load URL " << std::string(failedUrl)
        << " with error " << std::string(errorText) << " (" << errorCode << ").</h2></body></html>";
    frame->LoadURL(GetDataURI(ss.str(), "text/html"));
}