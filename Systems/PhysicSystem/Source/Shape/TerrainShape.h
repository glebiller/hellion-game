#pragma once

//
// extern includes
//
#pragma warning( push, 0 )
#pragma warning( disable : 6031 6201 6323 6386 )
#include "Common\Base\hkBase.h"
#include "Physics\Collide\Shape\HeightField\SampledHeightField\hkpSampledHeightFieldShape.h"
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