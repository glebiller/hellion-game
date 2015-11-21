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

#pragma once

#pragma warning( push, 0 )
#include <Overlay/OgreOverlay.h>
#include <OgreTexture.h>
#pragma warning( pop )

#include <include/cef_browser.h>
#include <include/cef_render_handler.h>

/**
* Implementation of the Surface.
*/
class BrowserRenderHandler : public CefRenderHandler {
public:
    /**
     * Default constructor.
     */
    BrowserRenderHandler();

    /**
     * Destructor.
     */
    ~BrowserRenderHandler();

    bool GetViewRect(CefRefPtr<CefBrowser> browser, CefRect &rect);

    void OnPaint(CefRefPtr<CefBrowser> browser, PaintElementType type, const RectList &dirtyRects, const void *buffer, int width, int height);
    
    IMPLEMENT_REFCOUNTING(BrowserRenderHandler);

private:
    Ogre::TexturePtr        m_texture;
    Ogre::MaterialPtr       m_material;
    Ogre::Overlay*          m_overlay;

};
