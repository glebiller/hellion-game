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

#include "Generic/IComponent.h"
#include "Generic/IObserver.h"
#include "Generic/IProperty.h"
#include "Generic/ISubject.h"
#include "Generic/IUpdatable.h"
#include "System/Changes.h"

class IEntity;
class ISystemScene;

/**
 * <c>ISystemObject</c> is an interface class designed to be an extension of the UObject.
 * Systems can extend the UObject by implementing this interface to give it new  properties.
 * An example would be a physics system implementing this interface so that the UObject would
 * now be able to interact with the physics system.
 *
 * @sa  CSubject
 * @sa  IObserver
 */
class ISystemObject : public IComponent, public ISubject, public IObserver, public IProperty, public IUpdatable {
public:

    /**
     * @inheritDoc
     */
    ISystemObject(ISystemScene* pSystemScene, IEntity* entity);

    /**
     * @inheritDoc
     */
    virtual ~ISystemObject(void);

    /**
     * @inheritDoc
     */
    void propertyChanged(System::Changes::BitMask uInChangedBits);

    /**
     * Gets the system scene this object belongs to.
     *
     * @return  A pointer to the system.
     */
    template <typename TSystemScene>
    inline TSystemScene* GetSystemScene(void) {
        return static_cast<TSystemScene*>(m_pSystemScene);
    }

    /**
     * Gets the entity of this object.
     *
     * @return  A pointer to the entity.
     */
    inline IEntity* getEntity(void) {
        return m_entity;
    }

protected:
    ISystemScene*               m_pSystemScene;
    IEntity*                    m_entity;

};