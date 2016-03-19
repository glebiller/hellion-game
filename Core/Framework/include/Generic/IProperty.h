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

#include <flatbuffers/flatbuffers.h>
#include <boost/function.hpp>
#include <map>
#include <string>

#include "DataTypes.h"
#include "System/Changes.h"

namespace Math { struct Quaternion; }
namespace Math { struct Vector3; }
namespace Math { struct Vector4; }

/**
 * Interface defining methods to Initialize & Set properties to an object.
 */
class IProperty {
public:

    IProperty();

    virtual ~IProperty();

    virtual Error initialize();

    /**
     * Method called to notify the object that a property has been changed.
     *
     * @param   uInChangedBits  The unsigned int bit field that describes the conceptual change with respect
     *                          to the published interests.
     */
    virtual void propertyChanged(System::Changes::BitMask uInChangedBits);

protected:
    bool                        m_bInitialized;
    System::Changes::BitMask    m_modified;

};
