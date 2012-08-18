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

#include <map>
#include <vector>
#include <string>
#include "boost/function.hpp"

#include "Errors.h"
#include "Property.h"
#include "System.h"
#include "System/ISystemSubject.h"

class ISystemTask;
class ISystemObject;

/**
 * <c>ISystemScene</c> is an interface class for managing a scene or scenes in a system.
 * 
 * @sa  CSubject
 * @sa  IObserver
 */
class ISystemScene : public ISystemSubject {

    public:

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
         * Called from the framework to inform the scene extension of the overall scene status.
         *
         * @param   Status  The overall scene status.
         */
        virtual void GlobalSceneStatusChanged(GlobalSceneStatus Status) {
            UNREFERENCED_PARAM(Status);
        };

        /**
         * Creates a system object used to extend a UObject.
         *
         * @param   pszName The unique name for this object.
         * @param   pszType The object type to create.
         * @return  The newly created system object.
         */
        virtual ISystemObject* CreateObject(const char* pszName, const char* pszType);

        /**
         * Destroys a system object.
         *
         * @param   pSystemObject   The system object to destroy.
         * @return  An error code.
         */
        virtual Error DestroyObject(ISystemObject* pSystemObject);

        /**
         * @inheritDoc
         */
        virtual Error ChangeOccurred(ISubject* pSubject, System::Changes::BitMask ChangeType);
        
        /**
         * Gets the system this object belongs to.
         *
         * @return  A pointer to the system.
         */
        ISystem* GetSystem(void) {
            return m_pSystem;
        }

        /**
         * Returns a pointer to the task that this scene needs to perform on its objects.
         *
         * @return  The task for this scene.
         */
        virtual ISystemTask* GetSystemTask(void) = 0;

    protected:

        typedef std::vector<ISystemObject*> ObjectsList;
        typedef boost::function<ISystemObject*(ISystemScene* pSystemScene, const char* pszName)> ObjectFactory;

        ISystem*                                m_pSystem;
        ObjectsList                             m_pObjects;
        std::map<std::string, ObjectFactory>    m_ObjectFactories;

};