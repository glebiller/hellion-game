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

#include "DataTypes.h"

namespace Errors {
    namespace System {
        static const u32 None                           = 0x0000;
        static const u32 Memory                         = 0x0001;
        static const u32 File                           = 0x0002;
    }

    static const u32 Undefined                          = (System::None << 16) | (0xffff & 0xffff);
    static const u32 Success                            = (System::None << 16) | (0x0000 & 0xffff);
    static const u32 Failure                            = (System::None << 16) | (0x0001 & 0xffff);

    static const u32 NotImplemented                     = (System::None << 16) | (0x0001 & 0xffff);

    namespace Memory {
        static const u32 OutOfMemory                    = (System::Memory << 16) | (0x0001 & 0xffff);
        static const u32 InvalidAddress                 = (System::Memory << 16) | (0x0002 & 0xffff);
    }

    namespace File {
        static const u32 ErrorLoading                   = (System::File << 16) | (0x0001 & 0xffff);
        static const u32 InvalidFormat                  = (System::File << 16) | (0x0002 & 0xffff);
        static const u32 NotFound                       = (System::File << 16) | (0x0003 & 0xffff);
    }
}
