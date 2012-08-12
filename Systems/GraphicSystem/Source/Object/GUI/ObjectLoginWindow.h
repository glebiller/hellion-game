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

#pragma once

#include "Object/GUI/ObjectWindow.h";

class ISystemScene;
class OGREGraphicsScene;

/**
 * Login window.  Display the login form.
 * 
 * @sa  GraphicObjectWindow
 */
class GraphicObjectLoginWindow : public GraphicObjectWindow {
        friend OGREGraphicsScene;

    protected:

        /**
         * Constructor.
         * Basic constructor; the only real one.
         *
         * @param   pSystemScene    ISystemScene* - Pointer to the scene containing this window.
         * @param   pszName         const char* - Name of this window.
         */
        GraphicObjectLoginWindow(ISystemScene* pSystemScene, const char* pszName);

        /**
         * Destructor.
         * Basic destructor; the only one.
         */
        ~GraphicObjectLoginWindow(void);

        /**
         * Updates the given DeltaTime.
         * Called by the framework every frame, so this window can update itself.
         *
         * @param   DeltaTime   f32 - Elapsed wall-clock time since the last call to this function.
         */
        virtual void Update(f32 DeltaTime);

    private:

        static const f32                    m_secondsPerUpdate;
        f32                                 m_secondsSinceLastUpdate;
        i32                                 m_framesSinceLastUpdate;
        WindowData                          m_windowData;

};
