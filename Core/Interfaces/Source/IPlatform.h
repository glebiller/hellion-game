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

/**
 * An interface for accessing platform specific functionality for things like the OS and
 *  processor.
 */
class IPlatform {
    public:

        /**
         * An interface for accessing processor information.
         */
        class IProcessor {
            public:

                /**
                 * Returns the number of processors available for this process.
                 *
                 * @return  The processor count.
                 */
                virtual u32 GetNumProcessors(void) = 0;

                /**
                 * Sets the calling threads affinity to the specified processor.
                 *
                 * @param   ProcessorNumber The processor number to set the affinity to.
                 */
                virtual void AffinitizeThreadToProcessor(u32 ProcessorNumber) = 0;
        };

        /**
         * Gets a reference to the IProcessor class.
         *
         * @return  A reference to the IProcessor class.
         */
        virtual IProcessor& Processor(void) = 0;

        /**
         * An interface for accessing window information.
         */
        class IWindow {
            public:

                /**
                 * Sets the window handle.
                 *
                 * @param   windowHnd   The window handle to set.
                 */
                virtual void SetHandle(size_t windowHnd) = 0;

                /**
                 * Returns the window handle.
                 *
                 * @return  The window handle.
                 */
                virtual size_t GetHandle(void) = 0;

                /**
                 * Sets a render window.
                 *
                 * @param   renderWindow    Handle of the render window.
                 */
                virtual void SetRenderWindow(Handle renderWindow) = 0;

                /**
                 * Gets the render window.
                 *
                 * @return  The render window.
                 */
                virtual Handle GetRenderWindow(void) = 0;

        };

        /**
         * Gets a reference to the IWindow class.
         *
         * @return  A reference to the IWindow class.
         */
        virtual IWindow& Window(void) = 0;

};
