#include <thread>
#include "include/wrapper/cef_helpers.h"
#include "json.hpp"
#include "common.h"
#include "msg_handler.h"

using json = nlohmann::json;

namespace
{

// can be called on any thread in the browser process
void SendIpcMessageToJs(CefRefPtr<CefBrowser> browser, const std::wstring &header, const json &body)
{
    std::wostringstream ss;
    ss << header << ": " << StringToWString(body.dump());
    auto msg = CefProcessMessage::Create(L"SendIpcMessageToJs");
    msg->GetArgumentList()->SetString(0, ss.str());
    browser->GetMainFrame()->SendProcessMessage(PID_RENDERER, msg);
}

}

bool MessageHandler::OnQuery(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, int64 query_id,
    const CefString& request, bool persistent, CefRefPtr<Callback> callback)
{
    std::wstring request_ws = request;
    auto comma_i = request_ws.find_first_of(L':');
    if (comma_i == std::wstring::npos || comma_i == request_ws.size()-1 || request_ws.at(comma_i+1) != L' ') {
        return false;
    }
    std::wstring header = request_ws.substr(0, comma_i);
    json body = json::parse(request_ws.substr(comma_i+2), nullptr, false);

    // handle based on header & json body
    if (header == L"IpcFromJsDemo") {
        if (!body.is_string()) {
            callback->Failure(0, L"json body should be string");
            return true;
        }
        auto echo = StringToWString(body.get<std::string>());
        std::reverse(echo.begin(), echo.end());
        json j1 = WStringToString(echo);
        std::thread([=] {
            std::this_thread::sleep_for(std::chrono::seconds(3));
            SendIpcMessageToJs(browser, L"IpcToJsDemo", body);
        }).detach();
        callback->Success(j1.dump());
        return true;
    }

    return false;
}