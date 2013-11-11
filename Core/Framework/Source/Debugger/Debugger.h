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

#include "Defines.h"

#if defined DEBUG_BUILD

#include <set>
#include <list>

#include "Proto/Debug/Debug.pb.h"

#include "SceneChangesDebugger.h"
#include "ObjectChangesDebugger.h"

#include <zmq.hpp>

class IChangeManager;
class ISystemObject;
class UScene;
class UObject;

/**
 * Debugger.
 *
 * @sa  Singleton
 */
class Debugger {
public:

    /**
     * Default constructor.
     */
    Debugger(void);

    /**
     * Destructor.
     */
    ~Debugger();

    /**
     * Initialises this object.
     *
     * @param   debuggerActive  true to debugger active.
     */
    void initialize(bool debuggerActive);

    /**
     * Sets the change managers references.
     *
     * @param [in,out]  pSceneCCM   If non-null, the scene ccm.
     * @param [in,out]  pObjectCCM  If non-null, the object ccm.
     */
    void setChangeManagers(IChangeManager* pSceneCCM, IChangeManager* pObjectCCM);

    /**
     * Sets u scene.
     *
     * @param [in,out]  pUScene If non-null, the u scene.
     */
    void setScene(const UScene* pUScene);

    /**
     * Cleans this object.
     */
    void clean(void);

    /**
     * Update the debugger.
     */
    void update(f32 deltaTime);

    /**
     * Send this message.
     *
     * @param [in,out]  debugHolderProto    If non-null, the debug holder prototype.
     */
    void send(DebugProto* debugHolderProto);

    /**
     * Add created object identifiers.
     *
     * @param objectId The created object identifiers.
     */
    void addCreatedObjectIds(std::string objectId);
    
    /**
     * Add updated object to the list.
     *
     * @param object The updated object.
     */
    void addUpdatedObject(ISubject* object);

    /**
     * Debug object.
     *
     * @param [in,out]  object      If non-null, the object.
     * @param [in,out]  debugProto  The debug prototype.
     */
    void debugObject(UObject* object, DebugProto& debugProto);

    /**
     * Debug object.
     *
     * @param [in,out]  object      If non-null, the object.
     * @param [in]      changes     The object changes.
     * @param [in,out]  debugProto  The debug prototype.
     */
    void debugObject(ISystemObject* object, DebugProto& debugProto);

private:

    IChangeManager*         m_pSceneCCM;
    IChangeManager*         m_pObjectCCM;
    
    SceneChangesDebugger*   m_pSceneChangesDebugger;
    ObjectChangesDebugger*  m_pObjectChangesDebugger;

    UScene*                 m_pUScene;
    std::list<std::string>  m_createdObjectIds;
    std::set<ISubject*>     m_updatedObjects;
    
    zmq::context_t*         m_pContext;
    zmq::socket_t*          m_pSocket;
    
    bool                    m_bDebuggerActive;

};

#endif
