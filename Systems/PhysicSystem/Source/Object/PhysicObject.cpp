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

#include "Object/PhysicObject.h"

#pragma warning( push, 0 )
#pragma warning( disable : 6031 6201 6323 6386 )
#include <Common\Base\hkBase.h>
#include <Common\Base\Types\Geometry\hkStridedVertices.h>
#include <Physics2012\Dynamics\Entity\hkpRigidBody.h>
#include <Physics2012\Collide\Shape\hkpShape.h>
#include <Physics2012\Collide\Shape\Convex\Box\hkpBoxShape.h>
#include <Physics2012\Collide\Shape\Convex\Sphere\hkpSphereShape.h>
#include <Physics2012\Collide\Shape\Convex\ConvexVertices\hkpConvexVerticesShape.h>
#include <Physics2012\Collide\Shape\Deprecated\Mesh\hkpMeshShape.h>
#include <Physics2012\Collide\Shape\Compound\Tree\Mopp\hkpMoppCompilerInput.h>
#include <Physics2012\Collide\Shape\Compound\Tree\Mopp\hkpMoppUtility.h>
#include <Physics2012\Collide\Shape\Compound\Tree\Mopp\hkpMoppBvTreeShape.h>
#include <Physics2012\Collide\Shape\HeightField\hkpHeightFieldShape.h>
#include <Physics2012\Collide\Shape\Misc\Bv\hkpBvShape.h>
#include <Physics2012\Collide\Shape\Misc\PhantomCallback\hkpPhantomCallbackShape.h>
#include <Physics2012\Utilities\Destruction\BreakOffParts\hkpBreakOffPartsUtil.h>
#pragma warning( pop )

#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>

#include "Scene.h"

hkVector4 PhysicObject::s_up = hkVector4(0, 1, 0);
hkVector4 PhysicObject::s_down = hkVector4(0,-1, 0);
hkVector4 PhysicObject::s_forward = hkVector4(1, 0, 0);
hkVector4 PhysicObject::s_gravity = hkVector4(0, -16, 0);

/**
 * @inheritDoc
 */
PhysicObject::PhysicObject(ISystemScene* pSystemScene, IEntity* entity) 
    : ISystemObject(pSystemScene, entity)
    , IGeometryObject()
    , m_pBody(nullptr)
    , m_bStatic(false) {
    m_orientation.Set(Math::Vector3::UnitZ, 0);
    
    m_propertySetters["Position"] = boost::bind(&IProperty::setVector3, this, System::Changes::Physic::Position, &m_position, _1);
    m_propertyGetters["Position"] = boost::bind(&IProperty::getVector3, this, &m_position, _1);
    
    m_propertySetters["Orientation"] = boost::bind(&IProperty::setQuaternion, this, System::Changes::Physic::Orientation, &m_orientation, _1);
    m_propertyGetters["Orientation"] = boost::bind(&IProperty::getQuaternion, this, &m_orientation, _1);

    m_propertySetters["Static"] = boost::bind(&IProperty::setSimpleType<bool>, this, System::Changes::None, &m_bStatic, _1);
    m_propertyGetters["Static"] = boost::bind(&IProperty::getSimpleType<bool>, this, &m_bStatic, _1);
}

/**
 * @inheritDoc
 */
PhysicObject::~PhysicObject(void) {

}
