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

#include "Defines.h"
#include "DataTypes.h"
#include "schema/system_type_generated.h"

#if defined( MSC_COMPILER )
#include <intrin.h>
#pragma intrinsic(_BitScanForward)
#endif

/**
 * The System namespace contains meta-data about the various sytems,
 * and various enums, datatypes and helper functions for system types.
 */
namespace System {
    /**
     * The Types namespace contains specifics about the system types.
     */
    namespace Types {
        static const unsigned int All = static_cast<unsigned int>(-1);
        static const unsigned int MAX = 32;

        /**
         * Get the index of the system with the given type ID.  Useful for looking up indexed properties.
         * This function works on both predefined and custom system type IDs.
         *
         * @param   SystemType  Type - The type ID of a system.
         * @return  unsigned int - Index of this system.
         */
        inline unsigned int GetIndex(const Schema::SystemType systemType) {
            unsigned int Index = All;
#if defined(MSC_COMPILER)
            _BitScanForward((unsigned long*)&Index, systemType);
#elif defined(GCC_COMPILER)
            Index = __builtin_ffs(static_cast<unsigned int>(systemType));
#endif
            return Index;
        };

        /**
         * Gets a type from an index.
         *
         * @param   SystemType  Type of the system.
         * @return  The type.
         */
        inline Schema::SystemType GetType(const unsigned int index) {
            return (Schema::SystemType) (1 << index);
        }

        typedef unsigned int BitMask;
    }
}
