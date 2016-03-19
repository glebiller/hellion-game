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

#include <UniversalScene_generated.h>
#include <SystemType_generated.h>
#include "DataTypes.h"

class ISystemScene;

/**
 * <c>ISystem</c> is an interface class designed to be a method for adding functionality to the
 *  framework.  By default the framework does not have functionality for things like graphics,
 *  physics, etc.
 */
class ISystem {
public:

    ISystem();

    virtual ~ISystem();

    /**
     * Creates a system scene for containing system objects.
     */
    virtual ISystemScene* createScene(const Schema::SystemScene* pScene) = 0;

    /**
     * Destroys a system scene.
     *
     * @param [in,out]  pSystemScene    The scene to destroy. Any objects within are destroyed.
     *
     * @return  An error code.
     */
    Error DestroyScene(ISystemScene* pSystemScene);

    virtual Schema::SystemType GetSystemType() = 0;

    /**
     * Gets system scene.
     *
     * @return  null if it fails, else the system scene.
     */
    template <typename TSystemScene>
    inline TSystemScene* getSystemScene() {
        return static_cast<TSystemScene*>(m_pSystemScene);
    }

    /**
     * Returns the CPU Utilization.
     *
     * @return  CPU Utilization (0-100f)
     */
    virtual float GetCPUUsage() {
        return 0;
    }

protected:
    ISystemScene*                           m_pSystemScene;

};