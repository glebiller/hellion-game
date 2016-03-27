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

#include "Object/AmbientLightGraphicObject.h"

#pragma warning( push, 0 )
// Temporarily switching warning level to 0 to ignore warnings in extern/Ogre
#include <Ogre.h>
#pragma warning( pop )

#include <schema/graphic_components_generated.h>

#include "GraphicScene.h"
#include "Object/LightGraphicObject.h"


/**
 * @inheritDoc
 */
AmbientLightGraphicObject::AmbientLightGraphicObject(ISystemScene& pSystemScene, UObject& entity,
                                                     const Schema::SystemComponent& component)
        : ISystemObject(&pSystemScene, &entity, component),
          sceneManager_(reinterpret_cast<GraphicScene&>(pSystemScene).getSceneManager()) {
    auto definition = static_cast<const Schema::Components::GraphicAmbientLight*> (component.data());
    auto lightColor = definition->color();
    Ogre::ColourValue colour = Ogre::ColourValue(lightColor->red(), lightColor->green(), lightColor->blue(), lightColor->alpha());
    sceneManager_->setAmbientLight(colour);
}

/**
 * @inheritDoc
 */
AmbientLightGraphicObject::~AmbientLightGraphicObject() {
}

/**
 * @inheritDoc
 */
void AmbientLightGraphicObject::Update(float DeltaTime) {

}

/**
 * @inheritDoc
 */
Error AmbientLightGraphicObject::ChangeOccurred(ISystemObject* systemObject, System::Changes::BitMask ChangeType) {
    return Errors::Success;
}
