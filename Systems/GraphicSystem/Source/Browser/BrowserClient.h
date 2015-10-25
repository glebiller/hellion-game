#pragma once

#include <cef_client.h>
#include <cef_render_handler.h>
#include <cef_browser_process_handler.h>
#include <wrapper/cef_message_router.h>

class BrowserRenderHandler;
class BrowserProcessHandler;

class BrowserClient : public CefClient {
public:
    BrowserClient(CefLifeSpanHandler* lifeSpanHandler, BrowserRenderHandler* renderHandler);

    ~BrowserClient(void);

    CefRefPtr<CefLifeSpanHandler> GetLifeSpanHandler() override;
    
    CefRefPtr<CefRenderHandler> GetRenderHandler() override;
    
    CefRefPtr<CefRequestHandler> GetRequestHandler() override;

    IMPLEMENT_REFCOUNTING(BrowserClient);

private:
    CefRefPtr<CefLifeSpanHandler> m_lifeSpanHandler;
    CefRefPtr<CefRenderHandler> m_renderHandler;
    CefRefPtr<CefMessageRouterBrowserSide> m_browserSideRouter;
    CefRefPtr<CefMessageRouterRendererSide> m_browserRouterRendererSide;
};

