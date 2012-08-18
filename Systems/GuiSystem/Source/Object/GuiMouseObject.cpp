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


//
// extern includes
//
#include "CEGUI.h"

//
// core includes
//
#include "BaseTypes.h"
#include "Interface.h"

//
// Ogre system includes
//
#include "GuiScene.h"
#include "Object/GuiMouseObject.h"

/**
 * @inheritDoc
 */
GuiMouseObject::GuiMouseObject(ISystemScene* pSystemScene, const char* pszName)
    : GuiObject(pSystemScene, pszName) {
    m_pszName = pszName;
}


/**
 * @inheritDoc
 */
GuiMouseObject::~GuiMouseObject(void) {

}

/**
 * @inheritDoc
 */
void GuiMouseObject::Update(f32 DeltaTime) {
    return;
}

/**
 * @inheritDoc
 */
Error GuiMouseObject::ChangeOccurred(ISubject* pSubject, System::Changes::BitMask ChangeType) {
    ASSERT(m_bInitialized);
    if (ChangeType & System::Changes::Input::Velocity) {
        IMoveObject* pMoveObject = dynamic_cast<IMoveObject*>(pSubject);
        CEGUI::System::getSingleton().injectMouseMove(pMoveObject->GetVelocity()->x, pMoveObject->GetVelocity()->y);
    }
    return Errors::Success;
}

/**
 * @inheritDoc
 */
Error GuiMouseObject::Initialize(std::vector<Properties::Property> Properties) {
    ASSERT(!m_bInitialized);
    m_bInitialized = true;
    return Errors::Success;
}

/**
 * @inheritDoc
 */
void GuiMouseObject::GetProperties(Properties::Array& Properties) {
    
}

/**
 * @inheritDoc
 */
void GuiMouseObject::SetProperties(Properties::Array Properties) {
    ASSERT(m_bInitialized);
}


