#include "json.hpp"
#include "common.h"
#include "msg_handler.h"

using json = nlohmann::json;

bool MessageHandler::OnQuery(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, int64 query_id,
    const CefString& request, bool persistent, CefRefPtr<Callback> callback)
{
    std::wstring message_name = request;
    if (message_name.find(L"MessageRouterTest") == 0) {
        std::wstring result = message_name.substr(18);
        std::reverse(result.begin(), result.end());
        json j;
        j["rtn"] = 0;
        j["msg"] = WStringToString(result);
        callback->Success(j.dump());
        return true;
    }
    return false;
}