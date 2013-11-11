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

#include <boost/container/flat_map.hpp>
#include <boost/function.hpp>
#include <map>
#include <string>

#include "DataTypes.h"
#include "Generic/IComponent.h"
#include "Generic/IObserver.h"
#include "Generic/IProperty.h"
#include "Generic/ISubject.h"
#include "Generic/IUpdatable.h"
#include "System/Changes.h"

class IEntity;
class ISystem;
class ISystemObject;
class ISystemTask;

/**
 * <c>ISystemScene</c> is an interface class for managing a scene or scenes in a system.
 */
class ISystemScene : public IComponent, public ISubject, public IObserver, public IProperty, public IUpdatable {

public:

    /**
     * Enumeration for informing the ISystemScene on what is occuring.
     * Pre and post loading are used to inform when the framework being and ends
     *        completely loading a scene.
     * Pre and post destroying are used to inform when the framework being and ends
     *        completely destroying a scene.
     */
    enum GlobalSceneStatus {
        PreLoadingObjects, PostLoadingObjects, PreDestroyingObjects, PostDestroyingObjects,
    };

    /**
     * Constructor.
     *
     * @param   pSystem The system this scene belongs to.
     */
    ISystemScene(ISystem* pSystem);
        
    /**
     * @inheritDoc
     */
    virtual ~ISystemScene(void);

    /**
     * Called from the framework to inform the scene extension of the overall scene status.
     *
     * @param   Status  The overall scene status.
     */
    virtual void GlobalSceneStatusChanged(GlobalSceneStatus Status);

    /**
     * @inheritDoc
     */
    virtual Error ChangeOccurred(ISubject* pSubject, System::Changes::BitMask ChangeType);

    /**
     * Create the system task for this scene.
     * 
     * @return A pointer to the system task.
     */
    void createTask(void);

    /**
     * Creates a system object used to extend a UObject.
     *
     * @param   entity The entity for this object.
     * @param   type The object type to create.
     * @return  The newly created system object.
     */
    ISystemObject* CreateObject(IEntity* entity, std::string type);

    /**
     * Destroys a system object.
     *
     * @param   pSystemObject   The system object to destroy.
     * @return  An error code.
     */
    Error DestroyObject(ISystemObject* pSystemObject);

    /**
     * @inheritDoc
     */
    void propertyChanged(System::Changes::BitMask uInChangedBits);
     
    /**
     * Gets the system this object belongs to.
     *
     * @return  A pointer to the system.
     */
    template <typename TSystem>
    TSystem* GetSystem(void) {
        return static_cast<TSystem*>(m_pSystem);
    }

    /**
     * Returns a pointer to the task that this scene needs to perform on its objects.
     *
     * @return  The task for this scene.
     */
    template <typename TSystemTask>
    TSystemTask* GetSystemTask(void) {
        return static_cast<TSystemTask*>(m_pSystemTask);
    }

protected:
    typedef boost::container::flat_map<std::string, ISystemObject*> ObjectsList;
    typedef boost::function<ISystemTask*(ISystemScene* pSystemScene)> TaskFactory;
    typedef boost::function<ISystemObject*(ISystemScene* pSystemScene, IEntity* entity)> ObjectFactory;

    ISystem*                                        m_pSystem;
    ISystemTask*							        m_pSystemTask;
    ObjectsList                                     m_pObjects;

    TaskFactory                                     m_TaskFactory;
    std::map<std::string, ObjectFactory>            m_ObjectFactories;

};