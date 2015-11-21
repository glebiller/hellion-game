﻿// Copyright � 2008-2009 Intel Corporation
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

#pragma once

#include <include/cef_browser.h>
#include <include/cef_life_span_handler.h>
#include <include/wrapper/cef_message_router.h>

/**
* Implementation of the Surface.
*/
class BrowserLifeSpanHandler : public CefLifeSpanHandler {
public:
    /**
     * Default constructor.
     */
    BrowserLifeSpanHandler(CefMessageRouterBrowserSide::Handler* handler);

    /**
     * Destructor.
     */
    ~BrowserLifeSpanHandler();

    void OnAfterCreated(CefRefPtr<CefBrowser> browser);

    void OnBeforeClose(CefRefPtr<CefBrowser> browser);
    
    IMPLEMENT_REFCOUNTING(BrowserLifeSpanHandler);

private:
    CefMessageRouterBrowserSide::Handler*       m_handler;
    CefRefPtr<CefMessageRouterBrowserSide>      m_message_router;
};
