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

    inline PerformanceHint getPerformanceHint(Schema::SystemType systemType) {
        // The indices of this array need to match System::TypeIndices.
        // Custom type indices are added here for convenience, but they are not guaranteed
        // to match (they're created in code with System::Types::MakeCustom()).
        //
        // If you modify System::TypeIndices or System::Types, please modify this to match.
        static PerformanceHint hints[static_cast<int>(Schema::SystemType::MAX) + 1] = {
            Task_NoPerformanceHint, // 0:  None
            Task_NoPerformanceHint, // 1:  Audio
            Task_LongSerial,        // 2:  Graphic
            Task_NoPerformanceHint, // 3:  Input
            Task_LongSerial,        // 4:  Network
            Task_LongSerial         // 5:  Physic
        };
        return hints[static_cast<int>(systemType)];
    };
}