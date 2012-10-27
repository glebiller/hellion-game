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

#pragma once

#include "DataTypes.h"
#include "MathUtils.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>
///   <c>IKeyboardObject</c> is an interface for providing Keyboard related functionality. Any
///    objects that modify or provide Keyboard data are required to implement this class.
/// </summary>
////////////////////////////////////////////////////////////////////////////////////////////////////
class IKeyboardObject {
    public:

		struct KeyboardButtonData {
            bool down;
		};

        /// <summary>
        /// Gets the keyboard button data.
        /// </summary>
        /// <returns>A constant to the keyboard button data.</returns>
        virtual const KeyboardButtonData GetKeyboardButtonData(void) = 0;

        /// <summary>
        /// Gets the mouse position data.
        /// </summary>
        /// <returns>A constant pointer to the mouse position data.</returns>
        virtual const Math::Vector3* GetMousePosition(void) = 0;

};
