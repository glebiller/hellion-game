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

#include <vector>

#include "Errors.h"
#include "Property.h"
#include "System.h"
#include "Observer/IObserver.h"
#include "Observer/CSubject.h"

class ISystemTask;
class ISystemObject;

/**
 * <c>ISystemScene</c> is an interface class for managing a scene or scenes in a system.
 * @sa  CSubject
 * @sa  CSubject
 * @sa  IObserver
 */
class ISystemScene : public CSubject, public IObserver {
        friend class ISystem;

    public:

        /**
         * Constructor.
         *
         * @param   pSystem The system this scene belongs to.
         */
        ISystemScene(ISystem* pSystem);

        /**
         * Gets the system this object belongs to.
         *
         * @return  A pointer to the system.
         */
        ISystem* GetSystem(void) {
            return m_pSystem;
        }

        /**
         * Gets the system type for this system scene.
         * This is a shortcut to getting the system type w/o having to go the system first.
         *
         * @return  The type of the system.
         */
        virtual SystemProto::Type GetSystemType(void) = 0;

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
         * One time initialization function for the scene.
         *
         * @param   Properties  Property structure array to fill in.
         * @return  An error code.
         */
        virtual Error Initialize(std::vector<Properties::Property> Properties) = 0;

        /**
         * Gets the properties of this scene.
         *
         * @param   Properties  Property structure array to fill.
         */
        virtual void GetProperties(std::vector<Properties::Property>& Properties) = 0;

        /**
         * Sets the properties for this scene.
         *
         * @param   Properties  Property structure array to get values from.
         */
        virtual void SetProperties(std::vector<Properties::Property> Properties) = 0;

        /**
         * Get all the available object types as names.
         *
         * @return  A NULL terminated array of object type names.
         */
        virtual const char** GetObjectTypes(void) = 0;

        /**
         * Creates a system object used to extend a UObject.
         *
         * @param   pszName The unique name for this object.
         * @param   pszType The object type to create.
         * @return  The newly created system object.
         */
        virtual ISystemObject* CreateObject(const char* pszName, const char* pszType) = 0;

        /**
         * Destroys a system object.
         *
         * @param   pSystemObject   The system object to destroy.
         * @return  An error code.
         */
        virtual Error DestroyObject(ISystemObject* pSystemObject) = 0;

        /**
         * Returns a pointer to the task that this scene needs to perform on its objects.
         *
         * @return  The task for this scene.
         */
        virtual ISystemTask* GetSystemTask(void) = 0;

        /**
         * Update the system scene object.
         *
         * @param   DeltaTime   Time of the delta.
         */
        virtual void Update(f32 DeltaTime) = 0;

        /**
         * Returns a bit mask of System Changes that this scene wants to receive changes for.  Used
         *  to inform the change control manager if this scene should be informed of the change.
         *
         * @return  A System::Changes::BitMask.
         */
        virtual System::Changes::BitMask GetDesiredSystemChanges(void) {
            return System::Changes::None;
        };

        /**
         * Change occurred.
         *
         * @param [in,out]  pSubject    If non-null, the subject.
         * @param   ChangeType          Type of the change.
         * @return  Error   If an error occured.
         */
        virtual Error ChangeOccurred(ISubject* pSubject, System::Changes::BitMask ChangeType) {
            UNREFERENCED_PARAM(pSubject);
            UNREFERENCED_PARAM(ChangeType);
            return Errors::Success;
        }

    protected:

        ISystem*                    m_pSystem;
        bool                        m_bInitialized;

};