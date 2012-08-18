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

#include "DataTypes.h"
#include "Errors.h"
#include "System/ISystem.h"
#include "System/ISystemScene.h"

/**
 * @inheritDoc
 */
ISystem::ISystem(void)
    : m_bInitialized(false) {

}

/**
 * @inheritDoc
 */
ISystem::~ISystem(void) {

}

/**
 * @inheritDoc
 */
const char* ISystem::GetName(void) {
    u32 index = System::Types::GetIndex(GetSystemType());
    SystemProto::Type systemType = static_cast<SystemProto::Type>(index);
    return SystemProto::Type_Name(systemType).c_str();
}

/**
 * @inheritDoc
 */
ISystemScene* ISystem::CreateScene(void) {
    return m_SceneFactory(this);
}

/**
 * @inheritDoc
 */
Error ISystem::DestroyScene(ISystemScene* pSystemScene) {
    ASSERT(pSystemScene != NULL);
    SAFE_DELETE(pSystemScene);
    return Errors::Success;
}

