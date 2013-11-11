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

#include "Defines.h"

/**
 * The System namespace contains meta-data about the various sytems,
 * and various enums, datatypes and helper functions for system types.
 */
namespace System {
    typedef u32 Change;

    /**
     * Defines the different changes as a bit mask that the systems can request
     * for other systems to perform. Custom changes are not allowed.
     */
    namespace Changes {
        typedef u32 BitMask;
        
        static const u32 None                   = 0;

        namespace Generic {
            static const u32 CreateObject       = (1 <<  0);
            static const u32 DeleteObject       = (1 <<  1);
            static const u32 All                = CreateObject | DeleteObject;
        }

        namespace Graphic {
        }

        namespace Input {
            static const u32 Velocity           = (1 << 10);
            static const u32 Rotation           = (1 << 11);
            static const u32 Action             = (1 << 12);
        }

        namespace Network {
        }

        namespace Physic {
            static const u32 Position           = (1 << 21);
            static const u32 Orientation        = (1 << 22);
        }

        static const u32 Link                   = (1 << 30);
        static const u32 ParentLink             = (1 << 31);
        static const u32 All                    = static_cast<u32>(-1);
    }
}
