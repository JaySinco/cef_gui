#pragma once
#include "include/wrapper/cef_message_router.h"

class MessageHandler: public CefMessageRouterBrowserSide::Handler
{
public:
    MessageHandler() {}

    bool OnQuery(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, int64 query_id,
        const CefString& request, bool persistent, CefRefPtr<Callback> callback) override;

private:
    DISALLOW_COPY_AND_ASSIGN(MessageHandler);
};