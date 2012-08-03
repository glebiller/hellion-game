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

#include "NormalizedLines.h"

NormalizedLines::NormalizedLines(pcstr pcstrMaterialName) {
    initialize(Ogre::RenderOperation::OT_LINE_LIST, false);
    setMaterial(pcstrMaterialName);
    m_bDirty = true;
}

NormalizedLines::~NormalizedLines() {
}

void NormalizedLines::addPoint(const Math::Vector3& pos, const Math::Vector3& norm) {
    m_Points.push_back(pos);
    m_Normals.push_back(norm);
    m_bDirty = true;
}

u16 NormalizedLines::getNumPoints(void) const {
    return (u16)m_Points.size();
}

void NormalizedLines::clear() {
    m_Points.clear();
    m_Normals.clear();
    m_bDirty = true;
}

void NormalizedLines::update() {
    if (m_bDirty) {
        fillHardwareBuffers();
    }
}

void NormalizedLines::createVertexDeclaration() {
    Ogre::VertexDeclaration* decl = mRenderOp.vertexData->vertexDeclaration;
    size_t offset = 0;
    decl->addElement(0, offset, Ogre::VET_FLOAT3, Ogre::VES_POSITION);
    offset += Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT3);
    decl->addElement(0, offset, Ogre::VET_FLOAT3, Ogre::VES_NORMAL);
}

void NormalizedLines::fillHardwareBuffers() {
    size_t size = m_Points.size();
    prepareHardwareBuffers(size, 0);

    if (!size) {
        mBox.setExtents(Ogre::Vector3::ZERO, Ogre::Vector3::ZERO);
        m_bDirty = false;
        return;
    }

    Math::Vector3 vaabMin = m_Points[0];
    Math::Vector3 vaabMax = m_Points[0];
    Ogre::HardwareVertexBufferSharedPtr vbuf =
        mRenderOp.vertexData->vertexBufferBinding->getBuffer(0);
    f32* prPos = static_cast<f32*>(vbuf->lock(Ogre::HardwareBuffer::HBL_DISCARD));
    {
        for (size_t i = 0; i < size; i++) {
            // Fill Position data
            *prPos++ = m_Points[i].x;
            *prPos++ = m_Points[i].y;
            *prPos++ = m_Points[i].z;
            // Fill Normal data
            *prPos++ = m_Normals[i].x;
            *prPos++ = m_Normals[i].y;
            *prPos++ = m_Normals[i].z;
            vaabMin.x = Math::Min(m_Points[i].x, vaabMin.x);
            vaabMin.y = Math::Min(m_Points[i].y, vaabMin.y);
            vaabMin.z = Math::Min(m_Points[i].z, vaabMin.z);
            vaabMax.x = Math::Max(m_Points[i].x, vaabMax.x);
            vaabMax.y = Math::Max(m_Points[i].y, vaabMax.y);
            vaabMax.z = Math::Max(m_Points[i].z, vaabMax.z);
        }
    }
    vbuf->unlock();
    mBox.setExtents(vaabMin.x, vaabMin.y, vaabMin.z,
                    vaabMax.x, vaabMax.y, vaabMax.z);
    m_bDirty = false;
}