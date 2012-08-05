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

#ifndef _NORMALIZED_LINES_H_
#define _NORMALIZED_LINES_H_

//
// core includes
//
#include "BaseTypes.h"

#include "MovableText/DynamicRenderable.h"
#include <vector>

// Class similar to DynamicLines, but specifically made to render the "Normal Ticks"
// and "Tangent Ticks" for visual debugging. The lines representing the normal ticks
// should be normalized to a fixed length, instead of being scaled

///////////////////////////////////////////////////////////////////////////////
/// <summary>
/// Class similar to DynamicLines, but specifically made to render the "Normal Ticks"
/// and "Tangent Ticks" for visual debugging. The lines representing the normal ticks
/// should be normalized to a fixed length, instead of being scaled
/// </summary>
///////////////////////////////////////////////////////////////////////////////
class NormalizedLines : public DynamicRenderable {
        typedef Ogre::RenderOperation::OperationType OperationType;

    public:
        /// Constructor - see setOperationType() for description of argument.
        NormalizedLines(const char* MaterialName = "BaseWhiteNoLighting");

        virtual ~NormalizedLines();

        /// <summary cref="NormalizedLines::addPoint">
        /// Add a normal to the list
        /// </summary>
        /// <param name="pos">const Math::Vector3& - Location of normal</param>
        /// <param name="norm">const Math::Vector3& - Normal vector</param>
        void addPoint(const Math::Vector3& pos, const Math::Vector3& norm);

        /// <summary cref="NormalizedLines::getNumPoints">
        /// Return the total number of normals
        /// </summary>
        /// <returns>u16 - number of normals</returns>
        u16 getNumPoints(void) const;

        /// <summary cref="NormalizedLines::clear">
        /// Remove all normals from the lise
        /// </summary>
        void clear();

        /// <summary cref="NormalizedLines::update">
        /// Call this to update the hardware buffer after making changes
        /// </summary>
        void update();

    protected:
        /// <summary cref="NormalizedLines::createVertexDeclaration">
        /// Implementation DynamicRenderable, creates a simple position and normal-only decl
        /// </summary>
        virtual void createVertexDeclaration();

        /// <summary cref="NormalizedLines::fillHardwareBuffers">
        /// Implementation DynamicRenderable, pushes point list out to hardware memory
        /// </summary>
        virtual void fillHardwareBuffers();

        std::vector<Math::Vector3> m_Points;
        std::vector<Math::Vector3> m_Normals;
        bool m_bDirty;
};


#endif

