// Copyright ? 2008-2009 Intel Corporation
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

#include <string>

#include "Defines.h"

/**
 * The System namespace contains meta-data about the various sytems,
 * and various enums, datatypes and helper functions for system types.
 */
namespace System {
    /**
     * The List of components
     */
    enum Components {
        System, 
        Scene, 
        Object, 
        Task
    };

#ifdef DEBUG_BUILD
    inline std::string getComponentName(const System::Components component) {
        switch (component) {
        case Components::System:
            return "System";
        case Components::Scene:
            return "Scene";
        case Components::Object:
            return "Object";
        case Components::Task:
            return "Task";
        default:
            return "Default";
        }
    }
#endif
}
