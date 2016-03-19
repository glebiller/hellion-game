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

#include <DataTypes.h>
#include "Defines.h"

/**
 * The System namespace contains meta-data about the various sytems,
 * and various enums, datatypes and helper functions for system types.
 */
namespace System {
    typedef unsigned int Change;

    /**
     * Defines the different changes as a bit mask that the systems can request
     * for other systems to perform. Custom changes are not allowed.
     */
    namespace Changes {
        typedef unsigned int BitMask;
        
        static const unsigned int None                   = 0;

        namespace Generic {
            static const unsigned int CreateObject       = (1 <<  0);
            static const unsigned int DeleteObject       = (1 <<  1);
            static const unsigned int All                = CreateObject | DeleteObject;
        }

        namespace Graphic {
        }

        namespace Input {
            static const unsigned int Velocity           = (1 << 10);
            static const unsigned int Rotation           = (1 << 11);
            static const unsigned int Action             = (1 << 12);
        }

        namespace Network {
        }

        namespace Physic {
            static const unsigned int Position           = (1 << 21);
            static const unsigned int Orientation        = (1 << 22);
        }

        static const unsigned int Link                   = (1 << 30);
        static const unsigned int ParentLink             = (1 << 31);
        static const unsigned int All                    = static_cast<unsigned int>(-1);
    }
}
