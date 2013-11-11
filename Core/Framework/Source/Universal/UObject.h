// Copyright © 2008-2009 Intel Corporation
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

#include "Generic/ISubject.h"
#include "Generic/IObserver.h"
#include "Generic/IEntity.h"
#include "System/ISystemScene.h"
#include "Object/IGeometryObject.h"

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
class UObject : public ISubject, public IObserver, public IEntity {
public:
    typedef std::map<Proto::SystemType, ISystemObject*>      SystemObjects;

    /**
     * Constructor.
     *
     * @param [in,out]  pScene  If non-null, the scene.
     * @param   id      The universal id of the object.
     * @param   name      The name of the object.
     */
    UObject(UScene* pScene, std::string id, std::string name, IEntity* parent);

    /**
     * Destructor.
     */
    ~UObject(void);

    /**
     * Used to extend the objects functionality for a given system.
     *   return - the newly created system object.
     *
     * @param [in,out]  pSystemScene    If non-null, the system scene.
     * @param   pszSystemObjectType     Type of the system object.
     *
     * @return  null if it fails, else.
     */
    ISystemObject* Extend(ISystemScene* pSystemScene, std::string pszSystemObjectType);

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
    const SystemObjects& GetExtensions(void);

    /**
     * Gets the specified system object.
     *  System - the system type of the object.
     *   return - this UOBject's system object for the specified type.
     *
     * @param   System  The system.
     *
     * @return  null if it fails, else the extension.
     */
    ISystemObject* GetExtension(Proto::SystemType System);

    /**
     * @inheritDoc
     */
    Error ChangeOccurred(ISubject* pSubject, System::Changes::BitMask SystemChanges);
    
    /**
     * @inheritDoc
     */
    inline System::Changes::BitMask GetDesiredSystemChanges(void) {
        return System::Changes::None;
    }
    
    /**
     * @inheritDoc
     */
    inline System::Changes::BitMask GetPotentialSystemChanges(void) {
        return System::Changes::None;
    }

protected:
    IChangeManager*                                     m_pObjectCCM;

    UScene*                                             m_pScene;
    SystemObjects                                       m_ObjectExtensions;

};
