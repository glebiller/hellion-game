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

#include "Property.h"
#include "System.h"

class ISystemScene;

/**
 * <c>ISystem</c> is an interface class designed to be a method for adding functionality to the
 *  framework.  By default the framework does not have functionality for things like graphics,
 *  physics, etc.
 */
class ISystem {
    public:

        /**
         * Constructor.
         */
        ISystem(void);

        /**
         * Gets the name of the system.  Only custom systems can return a custom name.
         * Non-custom system must return a pre-defined name in Systems::Names that matches with the type.
         *
         * @return  The name of the system.
         */
        virtual const char* GetName(void) = 0;

        /**
         * Gets the system type for this system.
         * This must function even w/o initialization.
         *
         * @return  The type of the system.
         */
        virtual System::Type GetSystemType(void) = 0;

        /**
         * One time initialization function for the system.
         *
         * @param   Properties  Property structure array to fill in.
         * @return  An error code.
         */
        virtual Error Initialize(std::vector<Properties::Property> Properties) = 0;

        /**
         * Gets the properties of this system.
         * Implementation must work prior to initialization.
         *
         * @param   Properties  Property structure array to fill.
         */
        virtual void GetProperties(std::vector<Properties::Property>& Properties) = 0;

        /**
         * Sets the properties for this system.
         *
         * @param   Properties  Property structure array to get values from.
         */
        virtual void SetProperties(std::vector<Properties::Property> Properties) = 0;

        /**
         * Creates a system scene for containing system objects.
         *
         * @return  The newly create system scene.
         */
        virtual ISystemScene* CreateScene(void) = 0;

        /**
         * Destroys a system scene.
         *
         * @param   pSystemScene    The scene to destroy. Any objects within are destroyed.
         * @return  An error code.
         */
        virtual Error DestroyScene(ISystemScene* pSystemScene) = 0;

        /**
         * Gets the CPU usage.
         * Returns the CPU Utilization of FMOD.
         *
         * @return  CPU Utilization (0-100f)
         */
        virtual f32 GetCPUUsage(void);


    protected:

        bool                        m_bInitialized;

};