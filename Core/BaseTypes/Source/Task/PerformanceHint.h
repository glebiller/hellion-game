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

#include "Proto/Common.pb.h"
#include "System/Types.h"

namespace Task {
    // PerformanceHint is used to modify the order that tasks are sent to the scheduler.
    // The order of definition is the order of scheduling.
    enum PerformanceHint {
        Task_LongSerial = 0,
        Task_LongParallel,
        Task_Short,
        Task_NoPerformanceHint,
        Task_MAX
    };

    inline PerformanceHint getPerformanceHint(Proto::SystemType systemType) {
        // The indices of this array need to match System::TypeIndices.
        // Custom type indices are added here for convenience, but they are not guaranteed
        // to match (they're created in code with System::Types::MakeCustom()).
        //
        // If you modify System::TypeIndices or System::Types, please modify this to match.
        static PerformanceHint hints[System::Types::MAX] = {
            Task_NoPerformanceHint, // 0:  Null
            Task_NoPerformanceHint, // 1:  Not used
            Task_NoPerformanceHint, // 2:  Generic
            Task_Short,             // 3:  Geometry
            Task_LongSerial,        // 4:  Graphics
            Task_LongSerial,        // 5:  PhysicsCollision
            Task_NoPerformanceHint, // 6:  Audio
            Task_NoPerformanceHint, // 7:  Input
            Task_LongParallel,      // 8:  AI
            Task_NoPerformanceHint, // 9:  Animation
            Task_Short,             // 10: Scripting
            Task_NoPerformanceHint, // 11: Explosion - test for ideal hint
            Task_NoPerformanceHint, // 12: Water - test for ideal hint
            Task_NoPerformanceHint, // 13: Not used
            Task_NoPerformanceHint, // 14: Not used
            Task_NoPerformanceHint, // 15: Not used
            Task_Short,             // 16: ProceduralTrees
            Task_NoPerformanceHint, // 17: Not used
            Task_LongParallel,      // 18: ProceduralFire

            // Not used below here, initialized for safety.
            Task_NoPerformanceHint, // 19: Not used
            Task_NoPerformanceHint, // 20: Not used
            Task_NoPerformanceHint, // 21: Not used
            Task_NoPerformanceHint, // 22: Not used
            Task_NoPerformanceHint, // 23: Not used
            Task_NoPerformanceHint, // 24: Not used
            Task_NoPerformanceHint, // 25: Not used
            Task_NoPerformanceHint, // 26: Not used
            Task_NoPerformanceHint, // 27: Not used
            Task_NoPerformanceHint, // 28: Not used
            Task_NoPerformanceHint, // 29: Not used
            Task_NoPerformanceHint, // 30: Not used
            Task_NoPerformanceHint  // 31: Not used
        };
        u32 jobIndex = System::Types::GetIndex(systemType);
        return hints[jobIndex];
    };
}