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
#include <OgreMeshManager.h>
#include <OgreSceneManager.h>
#include <OgreEntity.h>
#pragma warning( pop )

#include "BrowserRenderHandler.h"

/**
 * @inheritDoc
 */
BrowserRenderHandler::BrowserRenderHandler() {
    m_texture = Ogre::TextureManager::getSingleton().createManual("HUDTexture", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
       Ogre::TEX_TYPE_2D, 1024, 768, 0, Ogre::PF_BYTE_BGRA, Ogre::TU_DYNAMIC_WRITE_ONLY_DISCARDABLE);

    m_material = Ogre::MaterialManager::getSingleton().create("HUDMaterial", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
    m_material->getTechnique(0)->getPass(0)->createTextureUnitState("HUDTexture");
    m_material->getTechnique(0)->getPass(0)->setSceneBlending (Ogre::SBT_TRANSPARENT_ALPHA);
    //m_material->getTechnique(0)->getPass(0)->setCullingMode(Ogre::CULL_NONE);
    m_material->getTechnique(0)->getPass(0)->setLightingEnabled(false);

    Ogre::OverlayManager& overlayManager = Ogre::OverlayManager::getSingleton();
    m_overlay = overlayManager.create("OverlayName");

    Ogre::OverlayContainer* panel = static_cast<Ogre::OverlayContainer*>(overlayManager.createOverlayElement("Panel", "PanelName"));
    panel->setMetricsMode(Ogre::GuiMetricsMode::GMM_PIXELS);
    panel->setPosition(0.0, 0.0);
    panel->setDimensions(1024, 768);
    panel->setMaterialName("HUDMaterial");

    m_overlay->add2D(panel);
    m_overlay->show();
}

/**
 * @inheritDoc
 */
BrowserRenderHandler::~BrowserRenderHandler() {
    Ogre::OverlayManager::getSingleton().destroy(m_overlay);
    Ogre::MaterialManager::getSingleton().remove(m_material.get()->getHandle());
    Ogre::TextureManager::getSingleton().remove(m_texture.get()->getHandle());
}

/**
 * @inheritDoc.
 */
bool BrowserRenderHandler::GetViewRect(CefRefPtr<CefBrowser> browser, CefRect &rect) {
    rect = CefRect(0, 0, m_texture->getWidth(), m_texture->getHeight());
    return true;
}

/**
 * @inheritDoc.
 */
void BrowserRenderHandler::OnPaint(CefRefPtr<CefBrowser> browser, PaintElementType type, const RectList &dirtyRects, const void *buffer, int width, int height) {
    // TODO use dirtyRects ?
    Ogre::HardwarePixelBufferSharedPtr texBuf = m_texture->getBuffer();
    texBuf->lock(Ogre::HardwareBuffer::HBL_DISCARD);
    memcpy(texBuf->getCurrentLock().data, buffer, width * height * 4);
    texBuf->unlock();
}
