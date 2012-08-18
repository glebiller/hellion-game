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

#include "boost/function.hpp"
#include "System/ISystemScene.h"
#include "System/ISystem.h"
#include "System.h"

class GuiSystem;
class GuiTask;
class GuiObject;

/**
 * Implementation of the ISystemScene interface. See Interfaces\System.h for a definition of the
 * class and its functions.
 * 
 * @sa  ISystemScene
 */
class GuiScene : public ISystemScene {

        friend GuiSystem;
        friend GuiTask;

    public:

        /**
         * Constructor.
         *
         * @param [in,out]  pSystem If non-null, the system.
         */
        GuiScene(ISystem* pSystem);

        /**
         * Destructor.
         */
        ~GuiScene(void);

        /**
         * Gets the system type.
         * Implementation of the <c>ISystemScene::GetSystemType</c> function.
         *
         * @return  System::Type - Type of this system.
         *
         * @sa   ISystemScene::GetSystemType .
         */
        System::Type GetSystemType(void) {
            return System::Types::Gui;
        }

        /**
         * Global scene status changed.
         * Called from the framework to inform the scene extension of the overall scene status.
         *
         * @param   Status  GlobalSceneStatus - The overall scene status.
         */
        void GlobalSceneStatusChanged(GlobalSceneStatus Status);

        /**
         * Initializes this GuiScene.
         * Implementation of the <c>ISystemScene::Initialize</c> function. One time initialization
         * function for the scene.
         *
         * @param   Properties  Initializes the scene with the properties specified by
         *                      <paramref name="Properties"/>.
         * @return  Error.
         *
         * @sa   ISystemScene::Initialize    .
         */
        Error Initialize(Properties::Array Properties);

        /**
         * Gets the properties.
         * Implementation of the <c>ISystemScene::GetProperties</c> function. Gets the properties of
         * this scene.
         *
         * @param   Properties  Gets the properties of the scene.
         *
         * @sa   ISystemScene::GetProperties .
         */
        void GetProperties(Properties::Array& Properties);

        /**
         * Sets the properties.
         * Implementation of the <c>ISystemScene::SetProperties</c> function. Sets the properties for
         * this scene.
         *
         * @param   Properties  Sets the properties of the scene.
         *
         * @sa   ISystem::SetProperties  .
         */
        void SetProperties(Properties::Array Properties);

        /**
         * Gets the object types.
         * Implementation of the <c>ISystemScene::GetObjectTypes</c> function. Get all the available
         * object types as names.
         *
         * @return  const char** - A NULL terminated array of object type names.
         *
         * @sa   ISystemScene::GetObjectTypes    .
         */
        const char** GetObjectTypes(void);

        /**
         * Gets the system task.
         * Implementation of the <c>ISystemScene::GetSystemTask</c> function. Returns a pointer to the
         * task that this scene needs to perform on its objects.
         *
         * @return  ISystemTask* - The task for this scene.
         *
         * @sa   ISystemScene::GetSystemTask .
         */
        ISystemTask* GetSystemTask(void);

        /**
         * Gets the potential system changes.
         * Implementation of the <c>ISubject::GetPotentialSystemChanges</c> function. Identies the
         * system changes that this subject could possibly make.
         *
         * @return  System::Changes::BitMask - A bitmask of the possible system changes.
         *
         * @sa   ISubject::GetPotentialSystemChanges .
         */
        System::Changes::BitMask GetPotentialSystemChanges(void);

        /**
         * Gets a system change data.
         * Implementation of the <c>ISystemScene::GetDesiredSystemChanges</c> function. Returns a bit
         * mask of System Changes that this scene wants to receive changes for. Used to inform the
         * change control manager if this scene should be informed of the change.
         *
         * @param   SystemChange    The system change.
         * @return  System::Changes::BitMask - A bitmask of the desired system changes.
         *
         * @sa   ISystemScene::GetDesiredSystemChanges   .
         */
        const void* GetSystemChangeData(System::Change SystemChange);
        

        /**
         * Updates the given DeltaTime.
         * This function must be called every frame.  It updates the graphics scene.
         *
         * @param   DeltaTime   Elapsed time since the last frame.
         *
         * @sa   ISystemTask::Update .
         */
        void Update(f32 DeltaTime);

    protected:

        static const char*                      sm_kapszPropertyNames[];
        static const Properties::Property       sm_kaDefaultProperties[];

        GuiTask*                                m_pTask;

};

