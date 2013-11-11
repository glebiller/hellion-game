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

#include "MathUtils.h"

/**
 * <c>IGeometryObject</c> is an interface for providing geometry related functionality.  Any
 *  objects that modify geometry data are required to implement this class.
 */
class IGeometryObject {
public:

    /**
     * Default constructor.
     */
    IGeometryObject()
        : m_position(Math::Vector3::Zero)
        , m_orientation(Math::Quaternion::Zero)
        , m_scale(Math::Vector3::Zero) {
    }

    /**
     * Destructor.
     */
    virtual ~IGeometryObject() {
    }


    /**
     * Gets the position of the object.
     *
     * @return  A constant pointer to the position.
     */
    inline const Math::Vector3* GetPosition(void) {
        return &m_position;
    };

    /**
     * Gets the orientation of the object.
     *
     * @return  A constant pointer to the orientation.
     */
    inline const Math::Quaternion* GetOrientation(void) {
        return &m_orientation;
    };

    /**
     * Gets the scale of the object.
     *
     * @return  A constant pointer to the scle.
     */
    inline const Math::Vector3* GetScale(void) {
        return &m_scale;
    };
    
protected:
    Math::Vector3       m_position;
    Math::Quaternion    m_orientation;
    Math::Vector3       m_scale;

};
