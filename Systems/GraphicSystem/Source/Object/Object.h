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

#include "System.h"
#include "System/ISystemObject.h"
#include "SpinMutex.h"

class GraphicSystem;
class OGREGraphicsScene;

/**
 * Implementation of the IGraphicsObject interface. See Interfaces\Graphics.h and Interfaces\
 * System.h for a definition of the class and its functions.
 * 
 * @sa  ISystemObject
 */
class GraphicObject : public ISystemObject {
    
    public:

        enum Types {
            Type_Light,             // Light source (i.e. point light)
            Type_LightFire,         // Light source for a fire
            Type_Camera,            // Camera
            Type_Mesh,              // Model
            Type_MeshAnimated,      // A mesh with animation data
            Type_PointList,         // PointList particles
            Type_Window,            // A graphical window
            Type_StatWindow,        // Statistics window
            Type_Chart,             // A graphical chart
            Type_CPUChart,          // A graphical chart for showing CPU utilization
            Type_WorkloadWindow,    // Window for showing details on workload
            Type_ParticleSystem,    // Particle (Ogre3D particle system)
            Type_PagedGeometryLayer,// A layer on the paged geometry system
            Type_Terrain,           // The terrain layer
            Type_Sky,               // The Sky
            Type_Count,
        };
        
        /**
         * @inheritDoc
         */
        GraphicObject(ISystemScene* pSystemScene, const char* pszName);

        /**
         * @inheritDoc
         */
        virtual ~GraphicObject(void);

        /**
         * @inheritDoc
         */
        System::Type GetSystemType(void) {
            return System::Types::Graphic;
        }

        /**
         * @inheritDoc
         */
        virtual Error initialize(void);

        /**
         * @inheritDoc
         */
        virtual System::Changes::BitMask GetPotentialSystemChanges(void) {
            return System::Changes::None;
        };

        /**
         * @inheritDoc
         */
        virtual System::Types::BitMask GetDesiredSystemChanges(void) {
            return System::Changes::Geometry::Position |
                   System::Changes::Geometry::Orientation |
                   System::Changes::Geometry::Scale |
                   System::Changes::Graphics::GUI;
        };
        
        /**
         * @inheritDoc
         */
        virtual Error ChangeOccurred(ISubject* pSubject, System::Changes::BitMask ChangeType);

        /**
         * @inheritDoc
         */
        Types GetType(void) {
            return m_Type;
        }

    public:

        Types                               m_Type;
        static const char*                  sm_kapszTypeNames[];

        Ogre::SceneNode*                    m_pNode;
        DEFINE_SPIN_MUTEX(m_mutex);

};

