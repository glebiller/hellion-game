// Copyright 2008-2009 Intel Corporation
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

#pragma warning( push, 0 )
#pragma warning( disable : 6031 6201 6323 6386 )
#include <Common\Base\hkBase.h>
#include <Physics2012\Collide\Shape\HeightField\SampledHeightField\hkpSampledHeightFieldShape.h>
#pragma warning( pop )

class TerrainShape : public hkpSampledHeightFieldShape {
public:

    TerrainShape(const hkpSampledHeightFieldBaseCinfo& ci, hkUint16* data);

    HK_FORCE_INLINE hkReal getHeightAtImpl(int x, int z) const {
        return 100.0f * hkReal(m_data[x * m_zRes + z]) / hkReal(hkUint16(-1));
    }

    /// this should return true if the two triangles share the edge p00-p11
    /// otherwise it should return false if the triangles share the edge p01-p10
    HK_FORCE_INLINE hkBool getTriangleFlipImpl() const {
        return false;
    }

    /// Forward to collideSpheresImplementation
    virtual void collideSpheres(const CollideSpheresInput& input, SphereCollisionOutput* outputArray) const {
        return hkSampledHeightFieldShape_collideSpheres(*this, input, outputArray);
    }

private:
    hkUint16* m_data;

};