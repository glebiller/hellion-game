#include "BrowserClient.h"

#include "Browser/BrowserRenderHandler.h"
#include "Browser/BrowserLifeSpanHandler.h"

BrowserClient::BrowserClient(CefLifeSpanHandler* lifeSpanHandler, BrowserRenderHandler* renderHandler)
        : m_lifeSpanHandler(lifeSpanHandler)
        , m_renderHandler(renderHandler) {
    CefMessageRouterConfig config;
    config.js_query_function = "cefQuery";
    config.js_cancel_function = "cefQueryCancel";
    m_browserSideRouter = CefMessageRouterBrowserSide::Create(config);
    m_browserRouterRendererSide = CefMessageRouterRendererSide::Create(config);
}

BrowserClient::~BrowserClient() {
}

CefRefPtr<CefLifeSpanHandler> BrowserClient::GetLifeSpanHandler() {
    return m_lifeSpanHandler;
}

CefRefPtr<CefRenderHandler> BrowserClient::GetRenderHandler() {
    return m_renderHandler;
}

CefRefPtr<CefRequestHandler> BrowserClient::GetRequestHandler() {
    return NULL;
}