// Copyright © 2008-2009 Intel Corporation
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

#include "DynamicLines.h"
#pragma warning( push, 0 )
// Temporarily switching warning level to 0 to ignore warnings in extern/Ogre
#include <Ogre.h>
#pragma warning( pop )
#include <cassert>
#include <cmath>

using namespace Ogre;

enum {
    POSITION_BINDING,
    TEXCOORD_BINDING
};

DynamicLines::DynamicLines(OperationType opType, const char* pcstrMaterialName) {
    initialize(opType, false);
    setMaterial(pcstrMaterialName);
    mDirty = true;
}

DynamicLines::~DynamicLines() {
}

void DynamicLines::setOperationType(OperationType opType) {
    mRenderOp.operationType = opType;
}

RenderOperation::OperationType DynamicLines::getOperationType() const {
    return mRenderOp.operationType;
}

void DynamicLines::addPoint(const Vector3& p) {
    mPoints.push_back(p);
    mDirty = true;
}
void DynamicLines::addPoint(Real x, Real y, Real z) {
    mPoints.push_back(Vector3(x, y, z));
    mDirty = true;
}
const Vector3& DynamicLines::getPoint(unsigned short index) const {
    assert(index < mPoints.size() && "Point index is out of bounds!!");
    return mPoints[index];
}
unsigned short DynamicLines::getNumPoints(void) const {
    return (unsigned short)mPoints.size();
}
void DynamicLines::setPoint(unsigned short index, const Vector3& value) {
    assert(index < mPoints.size() && "Point index is out of bounds!!");
    mPoints[index] = value;
    mDirty = true;
}
void DynamicLines::clear() {
    mPoints.clear();
    mDirty = true;
}

void DynamicLines::update() {
    if (mDirty) { fillHardwareBuffers(); }
}

void DynamicLines::createVertexDeclaration() {
    VertexDeclaration* decl = mRenderOp.vertexData->vertexDeclaration;
    decl->addElement(POSITION_BINDING, 0, VET_FLOAT3, VES_POSITION);
}

void DynamicLines::fillHardwareBuffers() {
    size_t size = mPoints.size();
    prepareHardwareBuffers(size, 0);

    if (!size) {
        mBox.setExtents(Vector3::ZERO, Vector3::ZERO);
        mDirty = false;
        return;
    }

    Vector3 vaabMin = mPoints[0];
    Vector3 vaabMax = mPoints[0];
    HardwareVertexBufferSharedPtr vbuf =
        mRenderOp.vertexData->vertexBufferBinding->getBuffer(0);
    Real* prPos = static_cast<Real*>(vbuf->lock(HardwareBuffer::HBL_DISCARD));
    {
        for (size_t i = 0; i < size; i++) {
            *prPos++ = mPoints[i].x;
            *prPos++ = mPoints[i].y;
            *prPos++ = mPoints[i].z;

            if (mPoints[i].x < vaabMin.x)
            { vaabMin.x = mPoints[i].x; }

            if (mPoints[i].y < vaabMin.y)
            { vaabMin.y = mPoints[i].y; }

            if (mPoints[i].z < vaabMin.z)
            { vaabMin.z = mPoints[i].z; }

            if (mPoints[i].x > vaabMax.x)
            { vaabMax.x = mPoints[i].x; }

            if (mPoints[i].y > vaabMax.y)
            { vaabMax.y = mPoints[i].y; }

            if (mPoints[i].z > vaabMax.z)
            { vaabMax.z = mPoints[i].z; }
        }
    }
    vbuf->unlock();
    mBox.setExtents(vaabMin, vaabMax);
    mDirty = false;
}

/*
void DynamicLines::getWorldTransforms(Matrix4 *xform) const
{
   // return identity matrix to prevent parent transforms
   *xform = Matrix4::IDENTITY;
}
*/
/*
const Quaternion &DynamicLines::getWorldOrientation(void) const
{
   return Quaternion::IDENTITY;
}

const Vector3 &DynamicLines::getWorldPosition(void) const
{
   return Vector3::ZERO;
}
*/

