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

/**
 * File: MovableText.h
 *
 * description: This create create a billboarding object that display a text.
 *
 * @author  2003 by cTh see gavocanov@rambler.ru
 * @update  2006 by barraq see nospam@barraquand.com
 */


#ifndef __include_MovableText_H__
#define __include_MovableText_H__

namespace Ogre {

    ///////////////////////////////////////////////////////////////////////////////
    /// <summary>
    ///   Provides mechanisms for drawing text to the screen
    /// </summary>
    ///////////////////////////////////////////////////////////////////////////////
    class MovableText : public MovableObject, public Renderable {
        public:
            enum HorizontalAlignment    {H_LEFT, H_CENTER};
            enum VerticalAlignment      {V_BELOW, V_ABOVE};

        protected:
            String          mFontName;
            String          mType;
            String          mName;
            String          mCaption;
            HorizontalAlignment mHorizontalAlignment;
            VerticalAlignment   mVerticalAlignment;

            ColourValue     mColor;
            RenderOperation mRenderOp;
            AxisAlignedBox  mAABB;
            LightList       mLList;

            Real            mCharHeight;
            Real            mSpaceWidth;

            bool            mNeedUpdate;
            bool            mUpdateColors;
            bool            mOnTop;

            Real            mTimeUntilNextToggle;
            Real            mRadius;
            Real            mAdditionalHeight;

            Camera*          mpCam;
            RenderWindow*    mpWin;
            Font*            mpFont;
            MaterialPtr     mpMaterial;
            MaterialPtr     mpBackgroundMaterial;

        public:
            MovableText(const String& name, const String& caption, const String& fontName = "BlueHighway", Real charHeight = 1.0, const ColourValue& color = ColourValue::White);
            virtual ~MovableText();

            // Add to build on Shoggoth:
            virtual void visitRenderables(Ogre::Renderable::Visitor* visitor, bool debugRenderables = false);

            /// <summary cref="MovableText::setFontName">
            /// Set font name
            /// </summary>
            /// <param name="fontName">const String - Desired font</param>
            void    setFontName(const String& fontName);

            /// <summary cref="MovableText::setCaption">
            /// Set string (caption to be drawn)
            /// </summary>
            /// <param name="caption">const String - Desired caption to be drawn</param>
            void    setCaption(const String& caption);

            /// <summary cref="MovableText::setColor">
            /// Set string (caption)
            /// </summary>
            /// <param name="color">const ColourValue& - Desired color</param>
            void    setColor(const ColourValue& color);

            /// <summary cref="MovableText::setCharacterHeight">
            /// Set height of character
            /// </summary>
            /// <param name="height">Real - Desired height</param>
            void    setCharacterHeight(Real height);

            /// <summary cref="MovableText::setSpaceWidth">
            /// Set width of spaces
            /// </summary>
            /// <param name="height">Real - Desired space width</param>
            void    setSpaceWidth(Real width);

            /// <summary cref="MovableText::setTextAlignment">
            /// Set horizontal and vertical alignment of text
            /// </summary>
            /// <param name="horizontalAlignment">const HorizontalAlignment& - Desired horizontal alignment</param>
            /// <param name="horizontalAlignment">const VerticalAlignment& - Desired vertical alignment</param>
            void    setTextAlignment(const HorizontalAlignment& horizontalAlignment, const VerticalAlignment& verticalAlignment);

            /// <summary cref="MovableText::setAdditionalHeight">
            /// Set additional height (padding)
            /// </summary>
            /// <param name="height">Real - Desired additional height (padding)</param>
            void    setAdditionalHeight(Real height);

            /// <summary cref="MovableText::showOnTop">
            /// Enable/disable text being shown on top of everything
            /// </summary>
            /// <param name="show">bool - Show on top?</param>
            void    showOnTop(bool show = true);

            /// <summary cref="MovableText::getFontName">
            /// Get the current font name
            /// </summary>
            /// <returns>const String - Current font name</returns>
            const   String&          getFontName() const {return mFontName;}

            /// <summary cref="MovableText::getCaption">
            /// Get the current caption
            /// </summary>
            /// <returns>const String - Current caption</returns>
            const   String&          getCaption() const {return mCaption;}

            /// <summary cref="MovableText::getColor">
            /// Get the current font color
            /// </summary>
            /// <returns>const ColourValue - Current font color</returns>
            const   ColourValue&     getColor() const {return mColor;}

            /// <summary cref="MovableText::getCharacterHeight">
            /// Get the current character height
            /// </summary>
            /// <returns>float - current character height</returns>
            float    getCharacterHeight() const {return mCharHeight;}

            /// <summary cref="MovableText::getSpaceWidth">
            /// Get the current space width
            /// </summary>
            /// <returns>float - current space width</returns>
            float    getSpaceWidth() const {return mSpaceWidth;}

            /// <summary cref="MovableText::getAdditionalHeight">
            /// Get the current height padding
            /// </summary>
            /// <returns>float - current additional height</returns>
            float    getAdditionalHeight() const {return mAdditionalHeight;}

            /// <summary cref="MovableText::getShowOnTop">
            /// Returns true if the text is drawn on top of everything
            /// </summary>
            /// <returns>bool - drawn on top?</returns>
            bool    getShowOnTop() const {return mOnTop;}

            /// <summary cref="MovableText::GetAABB">
            /// Get axis aligned bounding box for the text
            /// </summary>
            /// <returns>AxisAlignedBox - axis aligned bounding of text</returns>
            AxisAlignedBox          GetAABB(void) { return mAABB; }

        protected:

            // from MovableText, create the object
            void    _setupGeometry();
            void    _updateColors();

            // from MovableObject
            void    getWorldTransforms(Matrix4* xform) const;
            Real    getBoundingRadius(void) const {return mRadius;};
            Real    getSquaredViewDepth(const Camera* cam) const { UNREFERENCED_PARAMETER(cam); return 0; };
            const   Quaternion&        getWorldOrientation(void) const;
            const   Vector3&           getWorldPosition(void) const;
            const   AxisAlignedBox&    getBoundingBox(void) const {return mAABB;};
            const   String&            getName(void) const {return mName;};
            const   String&            getMovableType(void) const {static Ogre::String movType = "MovableText"; return movType;};

            void    _notifyCurrentCamera(Camera* cam);
            void    _updateRenderQueue(RenderQueue* queue);

            // from renderable
            void    getRenderOperation(RenderOperation& op);
            const   MaterialPtr&       getMaterial(void) const {assert(!mpMaterial.isNull()); return mpMaterial;};
            const   LightList&         getLights(void) const {return mLList;};
    };

}

#endif
