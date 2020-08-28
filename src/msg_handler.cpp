#include "include/wrapper/cef_helpers.h"
#include "json.hpp"
#include "common.h"
#include "msg_handler.h"

using json = nlohmann::json;

namespace
{

void SendIpcMessageToJs(CefRefPtr<CefFrame> frame, const std::wstring &header, const json &body)
{
    CEF_REQUIRE_UI_THREAD();
    std::wostringstream ss;
    ss << header << ": " << StringToWString(body.dump());
    auto msg = CefProcessMessage::Create(L"SendIpcMessageToJs");
    msg->GetArgumentList()->SetString(0, ss.str());
    frame->SendProcessMessage(PID_RENDERER, msg);
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
    if (header == L"FromJs") {
        if (!body.is_string()) {
            callback->Failure(998, L"expect json string!");
            return true;
        }
        auto result = StringToWString(body.get<std::string>());
        std::reverse(result.begin(), result.end());
        json j = R"({"rtn": 0})"_json;
        j["msg"] = WStringToString(result);
        callback->Success(j.dump());
        SendIpcMessageToJs(browser->GetMainFrame(), L"FromCpp", R"("fuckme!")"_json);
        return true;
    }

    return false;
}