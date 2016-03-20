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

#include <boost/uuid/uuid.hpp>
#include <schema/scene_generated.h>

#include "System/ISystemObject.h"
#include "Generic/ISubject.h"
#include "Generic/IObserver.h"
#include "System/ISystemScene.h"

class IChangeManager;
class UScene;

/**
 * Implements a universal object that can be extended to add functionality.  By default this
 * universal object does not have any system functionality.
 *
 * @sa  ISubject
 * @sa  IObserver
 * @sa  IGeometryObject
 */
class UObject : public ISubject, public IObserver {
public:
    typedef std::map<Schema::SystemType, ISystemObject*>      SystemObjects;

    UObject(UScene* pScene, const Schema::SceneEntity& sceneEntity, UObject* parent);

    ~UObject();

    /**
     * Used to extend the objects functionality for a given system.
     *   return - the newly created system object.
     *
     * @param [in,out]  pSystemScene    If non-null, the system scene.
     * @param   pszSystemObjectType     Type of the system object.
     *
     * @return  null if it fails, else.
     */
    ISystemObject* Extend(ISystemScene* pSystemScene, const Schema::SystemComponent* component);

    /**
     * Used to unextend the objects functionality for a given system.
     *
     * @param [in,out]  pSystemScene    If non-null, the system scene.
     */
    void Unextend(ISystemScene* pSystemScene);

    /**
     * Returns the system objects map for this UObject.
     *
     * @return  A constant reference to the system object mapping.
     */
    const SystemObjects& GetExtensions();

    /**
     * Gets the specified system object.
     *  System - the system type of the object.
     *   return - this UOBject's system object for the specified type.
     *
     * @param   System  The system.
     *
     * @return  null if it fails, else the extension.
     */
    ISystemObject* GetExtension(Schema::SystemType System);

    /**
     * @inheritDoc
     */
    Error ChangeOccurred(ISubject* pSubject, System::Changes::BitMask SystemChanges) override;
    
    /**
     * @inheritDoc
     */
    inline System::Changes::BitMask GetDesiredSystemChanges() override {
        return System::Changes::None;
    }
    
    /**
     * @inheritDoc
     */
    inline System::Changes::BitMask GetPotentialSystemChanges() override {
        return System::Changes::None;
    }

    /**
     * Get the entity ID.
     */
    inline std::string getId() {
        auto name = entitySchema_->metaData()->entityId()->c_str();
        return entitySchema_->metaData()->entityId()->c_str();
    }

    /**
     * Get the entity name.
     */
    inline std::string getName() {
        auto name = entitySchema_->metaData()->name()->c_str();
        return entitySchema_->metaData()->name()->c_str();
    }

    /**
     * Get the entity parent.
     */
    inline UObject* getParent() {
        return parent_;
    }

    /**
     * Gets the children of this item.
     *
     * @return  null if it fails, else the children.
     */
    inline std::list<UObject*> getChildren() {
        return children_;
    }


protected:
    const Schema::SceneEntity*                          entitySchema_;

    IChangeManager*                                     m_pObjectCCM;

    UScene*                                             m_pScene;
    SystemObjects                                       m_ObjectExtensions; // EntityComponent
    UObject*                                            parent_;
    std::list<UObject*>                                 children_;

};
