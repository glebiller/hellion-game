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

#pragma warning( push, 0 )
#include <Overlay/OgreOverlayManager.h>
#include <Overlay/OgreOverlayContainer.h>
#include <OgreTexture.h>
#include <OgreMaterial.h>
#include <OgreTechnique.h>
#include <OgreHardwarePixelBuffer.h>
#include <OgreTextureManager.h>
#include <OgreMaterialManager.h>
#pragma warning( pop )

#include "BrowserWindow.h"

/**
 * @inheritDoc
 */
BrowserWindow::BrowserWindow() {
    m_texture = Ogre::TextureManager::getSingleton().createManual("HUDTexture", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
       Ogre::TEX_TYPE_2D, 500, 500, 0, Ogre::PF_BYTE_BGRA, Ogre::TU_DYNAMIC);

    Ogre::MaterialPtr material = Ogre::MaterialManager::getSingleton().create("HUDMaterial", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
    material->getTechnique(0)->getPass(0)->createTextureUnitState("HUDTexture");
    material->getTechnique(0)->getPass(0)->setSceneBlending(Ogre::SBT_TRANSPARENT_ALPHA);

    Ogre::OverlayManager& overlayManager = Ogre::OverlayManager::getSingleton();
    m_overlay = overlayManager.create("OverlayName");

    Ogre::OverlayContainer* panel = static_cast<Ogre::OverlayContainer*>(overlayManager.createOverlayElement("Panel", "PanelName"));
    panel->setPosition(0.0, 0.0);
    panel->setDimensions(1.0, 1.0);
    panel->setMaterialName("HUDMaterial");

    m_overlay->add2D(panel);
    m_overlay->show();
}

/**
 * @inheritDoc
 */
BrowserWindow::~BrowserWindow() {

}

/**
 * @inheritDoc.
 */
inline Ogre::Box BrowserWindow::rectToBox(Berkelium::Rect rect) {
    return Ogre::Box(rect.left(), rect.top(), 0, rect.right(), rect.bottom(), 1);
}

/**
 * @inheritDoc.
 */
void BrowserWindow::onPaint(Berkelium::Window *win, const unsigned char *sourceBuffer, const Berkelium::Rect &sourceBufferRect, size_t numCopyRects, const Berkelium::Rect *copyRects, int dx, int dy, const Berkelium::Rect &scrollRect) {
    const Ogre::HardwarePixelBufferSharedPtr textureBuffer = m_texture->getBuffer();
    const Ogre::PixelBox srcBox = Ogre::PixelBox(rectToBox(sourceBufferRect), Ogre::PF_BYTE_BGRA, const_cast<unsigned char*>(sourceBuffer));

    for(int i = 0; i < numCopyRects; i++) {
        const Ogre::Box destBox = rectToBox(copyRects[i]);
        textureBuffer->blitFromMemory(srcBox.getSubVolume(destBox), destBox);
    }
}

