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

#include <StdArg.h>

#include <BaseTypes.h>
#include "Interface.h"


namespace Interface {
    namespace Properties {

        Property::Property(
            pcstr pszName,
            u32 Type,
            u32 Flags,
            ...
        )
            : m_pszName(pszName)
            , m_Type(Type)
            , m_Flags(Flags)
            , m_apszEnumOptions(NULL) {
            //
            // Read in the ... passed in values.
            //
            va_list pArg;
            va_start(pArg, Flags);

            for (i32 i = 0; i < Values::Count; i++) {
                switch (Type & Values::Mask) {
                    case Values::None:
                        i = Values::Count;
                        break;

                    case Values::Boolean:
                        m_aValues[ i ].Boolean = va_arg(pArg, u32);
                        break;

                    case Values::Enum:
                    case Values::Int32:
                        m_aValues[ i ].Int32 = va_arg(pArg, i32);
                        break;

                    case Values::Float32:
                    case Values::Angle:
                        m_aValues[ i ].Float32 = va_arg(pArg, f32);
                        break;

                    case Values::Vector3& Values::Mask:
                    case Values::Color3& Values::Mask: {
                        ASSERTMSG(i == 0, "Vector3 or Color3 can be the only value on a property.");
                        m_aValues[ 0 ].Float32 = va_arg(pArg, f32);
                        m_aValues[ 1 ].Float32 = va_arg(pArg, f32);
                        m_aValues[ 2 ].Float32 = va_arg(pArg, f32);
                        i = Values::Count;
                        break;
                    }

                    case Values::Vector4& Values::Mask:
                    case Values::Quaternion& Values::Mask:
                    case Values::Color4& Values::Mask: {
                        ASSERTMSG(i == 0, "Vector4, Quaternion, or Color4 can be the only value on a property.");
                        m_aValues[ 0 ].Float32 = va_arg(pArg, f32);
                        m_aValues[ 1 ].Float32 = va_arg(pArg, f32);
                        m_aValues[ 2 ].Float32 = va_arg(pArg, f32);
                        m_aValues[ 3 ].Float32 = va_arg(pArg, f32);
                        i = Values::Count;
                        break;
                    }

                    case Values::String:
                    case Values::Path:
                        m_asValues[ i ] = va_arg(pArg, pcstr);
                        break;

                    default:
                        ASSERTMSG(False, "Unhandled property type.");
                        break;
                }

                Type >>= 8;
            }

            va_end(pArg);
        }
    }
}
