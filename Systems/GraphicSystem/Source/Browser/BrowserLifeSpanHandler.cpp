// Copyright � 2008-2009 Intel Corporation
// All Rights Reserved
//
// Permission is granted to use, copy, distribute and prepare derivative works of this
// software for any purpose and without fee, provided, that the above copyright notice
// and this statement appear in all copies.  Intel makes no representations about the
// suitability of this software for any purpose.  THIS SOFTWARE IS PROVIDED "AS IS."
// INTEL SPECIFICALLY DISCLAIMS ALL WARRANTIES, EXPRESS OR IMPLIED, AND ALL LIABILITY,
// INCLUDING CONSEQUENTIAL AND OTHER INDIRECT DAMAGES, FOR THE USE OF THIS SOFTWARE,
// INCLUDING LIABILITY FOR INFRINGEMENT OF ANY PROPRIETARY RIGHTS, AND INCLUDING THE
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.  Intel does not
// assume any responsibility for any errors which may appear in this software nor any
// responsibility to update it.

#include "BrowserLifeSpanHandler.h"

#include <include/cef_app.h>

/**
 * @inheritDoc
 */
BrowserLifeSpanHandler::BrowserLifeSpanHandler(CefMessageRouterBrowserSide::Handler* handler)
    : m_handler(handler) {

}

/**
 * @inheritDoc
 */
BrowserLifeSpanHandler::~BrowserLifeSpanHandler() {
}

/**
 * @inheritDoc.
 */
void BrowserLifeSpanHandler::OnAfterCreated(CefRefPtr<CefBrowser> browser) {
    if (!m_message_router) {
        // Create the browser-side router for query handling.
        CefMessageRouterConfig config;
        m_message_router = CefMessageRouterBrowserSide::Create(config);
        m_message_router->AddHandler(m_handler, false);
    }
}

void BrowserLifeSpanHandler::OnBeforeClose(CefRefPtr<CefBrowser> browser) {
  m_message_router->OnBeforeClose(browser);
}