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

#include "BaseTypes.h"
#include "Interface.h"

#include "Object.h"

/**
 * @inheritDoc
 */
GeometryObject::GeometryObject(ISystemScene* pSystemScene) : ISystemObject(pSystemScene, NULL)
    , m_Position(Math::Vector3::Zero)
    , m_Orientation(Math::Quaternion::Zero)
    , m_Scale(Math::Vector3::One) {

    //
    // Fill the properties default values
    //
    m_propertySetters["Position"] = boost::bind(&GeometryObject::setPosition, this, _1);
    m_propertySetters["Orientation"] = boost::bind(&GeometryObject::setOrientation, this, _1);
    m_propertySetters["Scale"] = boost::bind(&GeometryObject::setScale, this, _1);

}

/**
 * @inheritDoc
 */
GeometryObject::~GeometryObject(void) {

}

/**
 * @inheritDoc
 */
Error GeometryObject::initialize() {
    ASSERT(!m_bInitialized);
    return Errors::Success;
}

/**
 * @inheritDoc
 */
void GeometryObject::Update(f32 DeltaTime) {
    UNREFERENCED_PARAM(DeltaTime);
}

/**
 * @inheritDoc
 */
Error GeometryObject::ChangeOccurred(ISubject* pSubject, System::Changes::BitMask ChangeType) {
    ASSERT(m_bInitialized);
    if (ChangeType & System::Changes::Geometry::Position || ChangeType & System::Changes::Physics::Position) {
        m_Position = *dynamic_cast<IGeometryObject*>(pSubject)->GetPosition();
    }
    if (ChangeType & System::Changes::Geometry::Orientation) {
        m_Orientation = *dynamic_cast<IGeometryObject*>(pSubject)->GetOrientation();
    }
    if (ChangeType & System::Changes::Geometry::Scale) {
        m_Scale = *dynamic_cast<IGeometryObject*>(pSubject)->GetScale();
    }
    return Errors::Success;
}

/**
 * @inheritDoc
 */
void GeometryObject::setPosition(ProtoStringList values) {
    ProtoStringList::const_iterator value = values.begin();
    m_Position.x = *(value++);
    m_Position.y = *(value++);
    m_Position.z = *value;
    PostChanges(System::Changes::Geometry::Position);
}

/**
 * @inheritDoc
 */
void GeometryObject::setOrientation(ProtoStringList values) {
    ProtoStringList::const_iterator value = values.begin();
    m_Orientation.x = *(value++);
    m_Orientation.y = *(value++);
    m_Orientation.z = *(value++);
    m_Orientation.w = *value;
    PostChanges(System::Changes::Geometry::Orientation);
}

/**
 * @inheritDoc
 */
void GeometryObject::setScale(ProtoStringList values) {
    ProtoStringList::const_iterator value = values.begin();
    m_Scale.x = *(value++);
    m_Scale.y = *(value++);
    m_Scale.z = *value;
    PostChanges(System::Changes::Geometry::Scale);
}