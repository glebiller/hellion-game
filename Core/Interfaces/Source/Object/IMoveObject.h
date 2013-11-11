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
#include "MathUtils.h"

/**
 * <c>IMoveObject</c> is an interface for providing Move related functionality.  Any
 *  objects that modify or provide Move data are required to implement this class.
 */
class IMoveObject {
public:

    /**
     * Default constructor.
     */
    IMoveObject()
        : m_velocity(Math::Vector3::Zero)
        , m_rotation(Math::Vector3::Zero) {
    }

    /**
     * Destructor.
     */
    virtual ~IMoveObject() {
    }

    /**
     * Gets the velocity of the object.
     *
     * @return  A constant pointer to the velocity.
     */
    inline const Math::Vector3* getVelocity(void) {
        return &m_velocity;
    }

    /**
     * Gets the rotation.
     *
     * @return  null if it fails, else the rotation.
     */
    inline const Math::Vector3* getRotation(void) {
        return &m_rotation;
    }

protected:
    Math::Vector3       m_velocity;
    Math::Vector3       m_rotation;

};
