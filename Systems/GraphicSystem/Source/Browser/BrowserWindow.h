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

#include <berkelium/WindowDelegate.hpp>

/**
* Implementation of the Surface.
*/
class BrowserWindow : public Berkelium::WindowDelegate {
public:
    /**
     * Default constructor.
     */
    BrowserWindow();

    /**
     * Destructor.
     */
    ~BrowserWindow(void);

    /**
     * Rectangle to box.
     *
     * @param   rect    The rectangle.
     *
     * @return  .
     */
    inline Ogre::Box rectToBox(Berkelium::Rect rect);

    /**
     * @inheritDoc
     */
    virtual void onPaint(Berkelium::Window *win, const unsigned char *sourceBuffer, const Berkelium::Rect &sourceBufferRect, size_t numCopyRects, 
        const Berkelium::Rect *copyRects, int dx, int dy, const Berkelium::Rect &scrollRect);

private:
    Ogre::TexturePtr        m_texture;
    Ogre::Overlay*          m_overlay;

};
