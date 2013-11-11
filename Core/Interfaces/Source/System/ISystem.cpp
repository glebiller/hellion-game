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

#include <boost/checked_delete.hpp>

#include "DataTypes.h"
#include "Errors.h"
#include "System/ISystem.h"
#include "System/ISystemScene.h"

/**
 * @inheritDoc
 */
ISystem::ISystem() : IProperty() {

}

/**
 * @inheritDoc
 */
ISystem::~ISystem() {

}

/**
 * @inheritDoc
 */
void ISystem::createScene() {
    m_pSystemScene = m_SceneFactory(this);
}

/**
 * @inheritDoc
 */
Error ISystem::DestroyScene(ISystemScene* pSystemScene) {
    boost::checked_delete(pSystemScene);
    return Errors::Success;
}

