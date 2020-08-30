#include <thread>
#include "include/wrapper/cef_helpers.h"
#include "common.h"
#include "msg_handler.h"
#define QUERY_INVALID_HEADER 0
#define QUERY_INVALID_BODY 1

bool MessageHandler::OnQuery(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, int64 query_id,
    const CefString& request, bool persistent, CefRefPtr<Callback> callback)
{
    std::wstring request_ws = request;
    auto comma_i = request_ws.find_first_of(L':');
    if (comma_i == std::wstring::npos || comma_i == request_ws.size()-1 || request_ws.at(comma_i+1) != L' ') {
        callback->Failure(QUERY_INVALID_HEADER, "request should be like `header: [body]`");
        return true;
    }
    std::wstring header = request_ws.substr(0, comma_i);
    std::wstring body_str = request_ws.substr(comma_i+2);
    bool has_no_body = body_str.size() == 0;
    json body = json::parse(request_ws.substr(comma_i+2), nullptr, false);
    if (!has_no_body && body.is_discarded()) {
        callback->Failure(QUERY_INVALID_BODY, "request body can't be parsed as json");
        return true;
    }

    // handle based on header & json body
    if (header == L"get_resdir") {
        if (!has_no_body) {
            callback->Failure(QUERY_INVALID_BODY, "request should has no body");
        }
        else {
            callback->Success(get_resdir());
        }
        return true;
    }

    callback->Failure(QUERY_INVALID_HEADER, "request header unimplemented");
    return true;
}

void MessageHandler::SendIpcMessageToJs(CefRefPtr<CefBrowser> browser, const std::wstring &header,
    const json &body)
{
    std::wostringstream ss;
    ss << header << ": ";
    if (!body.empty()) {
        ss << s2ws(body.dump());
    }
    auto msg = CefProcessMessage::Create(L"SendIpcMessageToJs");
    msg->GetArgumentList()->SetString(0, ss.str());
    browser->GetMainFrame()->SendProcessMessage(PID_RENDERER, msg);
}
