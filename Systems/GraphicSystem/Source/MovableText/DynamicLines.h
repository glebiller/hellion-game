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

#ifndef _DYNAMIC_LINES_H_
#define _DYNAMIC_LINES_H_

//
// core includes
//
#include "BaseTypes.h"

#include "DynamicRenderable.h"
#include <vector>

///////////////////////////////////////////////////////////////////////////////
/// <summary>
///   Class to provide a DynamicRenderable wrapper for a drawing lines.
/// </summary>
///////////////////////////////////////////////////////////////////////////////
class DynamicLines : public DynamicRenderable {
        typedef Ogre::RenderOperation::OperationType OperationType;

    public:
        /// Constructor - see setOperationType() for description of argument.
        DynamicLines(OperationType opType = Ogre::RenderOperation::OT_LINE_STRIP, const char* MaterialName = "BaseWhiteNoLighting");
        virtual ~DynamicLines();

        /// <summary cref="DynamicLines::addPoint">
        /// Add a point to the point list (vector)
        /// </summary>
        /// <param name="p">Ogre::Vector3& - Point to add.</param>
        void addPoint(const Ogre::Vector3& p);

        /// <summary cref="DynamicLines::addPoint">
        /// Add a point to the point list
        /// </summary>
        /// <param name="x">f32 - Point x value</param>
        /// <param name="y">f32 - Point y value</param>
        /// <param name="z">f32 - Point z value</param>
        void addPoint(f32 x, f32 y, f32 z);

        /// <summary cref="DynamicLines::setPoint">
        /// Change the location of an existing point in the point list
        /// </summary>
        /// <param name="index">unsigned short - Index of point to update</param>
        /// <param name="value">Ogre::Vector3& - New point location</param>
        void setPoint(unsigned short index, const Ogre::Vector3& value);

        /// <summary cref="DynamicLines::getPoint">
        /// Return the location of an existing point in the point list
        /// </summary>
        /// <param name="index">unsigned short - Index of point to return</param>
        /// <returns>Ogre::Vector3& - Point location</returns>
        const Ogre::Vector3& getPoint(unsigned short index) const;

        /// <summary cref="DynamicLines::getNumPoints">
        /// Return the total number of points in the point list
        /// </summary>
        /// <returns>unsigned short - Number of points</returns>
        unsigned short getNumPoints(void) const;

        /// <summary cref="DynamicLines::clear">
        /// Remove all points from the point list
        /// </summary>
        void clear();

        /// <summary cref="DynamicLines::update">
        /// Call this to update the hardware buffer after making changes.
        /// </summary>
        void update();

        /// <summary cref="DynamicLines::setOperationType">
        /// Set the type of operation to draw with.
        /// </summary>
        /// <param name="opType">opType Can be one of
        ///  RenderOperation::OT_LINE_STRIP,
        ///  RenderOperation::OT_LINE_LIST,
        ///  RenderOperation::OT_POINT_LIST,
        ///  RenderOperation::OT_TRIANGLE_LIST,
        ///  RenderOperation::OT_TRIANGLE_STRIP,
        ///  RenderOperation::OT_TRIANGLE_FAN...
        ///  The default is OT_LINE_STRIP. </param>
        void setOperationType(OperationType opType);

        /// <summary cref="DynamicLines::getOperationType">
        /// Returns mRenderOp.operationType
        /// </summary>
        /// <returns>Ogre::RenderOperation::OperationType</returns>
        OperationType getOperationType() const;

    protected:
        /// Implementation DynamicRenderable, creates a simple vertex-only decl
        virtual void createVertexDeclaration();
        /// Implementation DynamicRenderable, pushes point list out to hardware memory
        virtual void fillHardwareBuffers();

        std::vector<Ogre::Vector3> mPoints;
        bool mDirty;
};


#endif

