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

#ifndef DYNAMIC_RENDERABLE_H
#define DYNAMIC_RENDERABLE_H

#pragma warning( push, 0 )
// Temporarily switching warning level to 0 to ignore warnings in extern/Ogre
#include <OgreSimpleRenderable.h>
#pragma warning( pop )

///////////////////////////////////////////////////////////////////////////////
/// <summary>
///   Abstract base class providing mechanisms for dynamically growing hardware buffers.
/// </summary>
///////////////////////////////////////////////////////////////////////////////
class DynamicRenderable : public Ogre::SimpleRenderable {
    public:
        /// Constructor
        DynamicRenderable();
        /// Virtual destructor
        virtual ~DynamicRenderable();

        /// <summary cref="DynamicRenderable::initialize">
        /// Initializes the dynamic renderable.
        ///
        /// This function should only be called once. It initializes the
        /// render operation, and calls the abstract function
        /// createVertexDeclaration().
        /// </summary>
        /// <param name="operationType">Ogre::RenderOperation::OperationType - The type of render operation to perform.</param>
        /// <param name="useIndices">bool - Specifies whether to use indices to determine the vertices to use as input.</param>
        void initialize(Ogre::RenderOperation::OperationType operationType, bool useIndices);

        /// <summary cref="DynamicRenderable::getBoundingRadius">
        /// Implementation of Ogre::SimpleRenderable getBoundingRadius
        /// </summary>
        virtual Ogre::Real getBoundingRadius(void) const;

        /// <summary cref="DynamicRenderable::getSquaredViewDepth">
        /// Implementation of Ogre::SimpleRenderable getSquaredViewDepth
        /// </summary>
        virtual Ogre::Real getSquaredViewDepth(const Ogre::Camera* cam) const;

    protected:
        /// Maximum capacity of the currently allocated vertex buffer.
        size_t mVertexBufferCapacity;
        /// Maximum capacity of the currently allocated index buffer.
        size_t mIndexBufferCapacity;

        /// <summary cref="DynamicRenderable::createVertexDeclaration">
        /// Creates the vertex declaration.
        ///
        /// Override and set mRenderOp.vertexData->vertexDeclaration here.
        /// mRenderOp.vertexData will be created for you before this method
        /// is called.
        /// </summary>
        virtual void createVertexDeclaration() = 0;

        /// <summary cref="DynamicRenderable::prepareHardwareBuffers">
        /// Prepares the hardware buffers for the requested vertex and index counts.
        ///
        /// This function must be called before locking the buffers in
        /// fillHardwareBuffers(). It guarantees that the hardware buffers
        /// are large enough to hold at least the requested number of
        /// vertices and indices (if using indices). The buffers are
        /// possibly reallocated to achieve this.
        ///
        /// The vertex and index count in the render operation are set to
        /// the values of vertexCount and indexCount respectively.
        /// </summary>
        /// <param name="vertexCount">size_t - The number of vertices the buffer must hold.</param>
        /// <param name="indexCount">size_t - The number of indices the buffer must hold. This parameter is ignored if not using indices.</param>
        void prepareHardwareBuffers(size_t vertexCount, size_t indexCount);

        /// <summary cref="DynamicRenderable::fillHardwareBuffers">
        /// Fills the hardware vertex and index buffers with data.
        ///
        /// This function must call prepareHardwareBuffers() before locking
        /// the buffers to ensure the they are large enough for the data to
        /// be written. Afterwards the vertex and index buffers (if using
        /// indices) can be locked, and data can be written to them.
        /// </summary>
        virtual void fillHardwareBuffers() = 0;
};

#endif // DYNAMIC_RENDERABLE_H

