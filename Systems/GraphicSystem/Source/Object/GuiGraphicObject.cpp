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
// Temporarily switching warning level to 0 to ignore warnings in extern/Ogre
#include <Ogre.h>
#pragma warning( pop )

#pragma warning( push, 0 )
#pragma warning( disable : 4244 6211 6386 )

#include "Manager/ServiceManager.h"
#include "Scene.h"
#include "System.h"
#include "Object.h"
#include "Object/GuiGraphicObject.h"
#include "Object/IMoveObject.h"
#include "Browser/BrowserRenderHandler.h"
#include "Browser/BrowserLifeSpanHandler.h"

#define POGRESCENEMGR (reinterpret_cast<GraphicScene*>(m_pSystemScene)->getSceneManager())

extern IServiceManager* g_serviceManager;

/**
 * @inheritDoc
 */
GuiGraphicObject::GuiGraphicObject(ISystemScene* pSystemScene, IEntity* entity) 
    : GraphicObject(pSystemScene, entity)
    , m_browserClient(new BrowserClient(new BrowserLifeSpanHandler(this), new BrowserRenderHandler())) {
    //m_propertySetters["Mesh"] = boost::bind(&MeshGraphicObject::setMeshName, this, _1);
}

/**
 * @inheritDoc
 */
GuiGraphicObject::~GuiGraphicObject() {
    m_browser = nullptr;
    m_browserClient = nullptr;
}

/**
 * @inheritDoc
 */
Error GuiGraphicObject::initialize() {
    ASSERT(!m_bInitialized);
    
    CefWindowInfo window_info;
    //window_info.SetAsWindowless((HWND) g_serviceManager->getWindowService()->getHandle(), true);
    CefBrowserSettings browserSettings;
    m_browser = CefBrowserHost::CreateBrowserSync(window_info, m_browserClient.get(), "file:///Assets/Media/Gui/index.html", browserSettings, nullptr);
    m_browser->GetHost()->SendFocusEvent(true);
    g_serviceManager->getWindowService()->setBrowser(&m_browser);

    m_bInitialized = true;
    return Errors::Success;
}

/**
 * @inheritDoc
 */
void GuiGraphicObject::Update(f32 DeltaTime) {

}

/**
 * @inheritDoc
 */
Error GuiGraphicObject::ChangeOccurred(ISubject* pSubject, System::Changes::BitMask ChangeType) {
    ASSERT(m_bInitialized);
    
    return Errors::Success;
}

bool GuiGraphicObject::OnQuery(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, int64 query_id, const CefString& request, bool persistent, CefRefPtr<Callback> callback) {
    const std::string& message_name = request;
    if (message_name.find("login") == 0) {
      bool clicked = true;
      assert(clicked);
    } else {
      NOTREACHED();
    }

    callback->Success("");
    return true;
  }

#pragma warning( pop )
