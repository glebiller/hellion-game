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

#include <SpinMutex.h>

class GraphicSystem;
class OGREGraphicsScene;

///////////////////////////////////////////////////////////////////////////////
/// <summary>
///   Implementation of the IGraphicsObject interface.
///   See Interfaces\Graphics.h and Interfaces\System.h for a definition of the
///   class and its functions.
/// </summary>
///////////////////////////////////////////////////////////////////////////////
class GraphicObject : public ISystemObject {
        friend OGREGraphicsScene;


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

        /// <summary cref="GraphicObject::GetType">
        ///   Returns the type of graphics object
        /// </summary>
        /// <returns>GraphicObject::Types - Type object</returns>
        Types GetType(void) {
            return m_Type;
        }

        /// <summary cref="GraphicObject::Update">
        ///   Implementation of the <c>ISystemTask::Update</c> method.
        /// </summary>
        /// <param name="DeltaTime">Elapsed time since the last frame.</param>
        /// <seealso cref="ISystemTask::Update"/>
        virtual void Update(f32 DeltaTime);

    protected:

        GraphicObject(ISystemScene* pSystemScene, pcstr pszName);
        ~GraphicObject(void);

        /// <summary cref="GraphicObject::GetSystemType">
        ///   Implementation of the <c>ISystemObject::GetSystemType</c> function.
        ///   Lets this object know when a registered aspects of interest has changed
        ///   (this function will be called when other systems make changes this object should know about).
        /// </summary>
        /// <returns>System::Type - Type of this system.</returns>
        /// <seealso cref="ISystemObject::GetSystemType"/>
        virtual System::Type GetSystemType(void);

        /// <summary cref="GraphicObject::Initialize">
        ///   Implementation of the <c>ISystem::Initialize</c> function.
        /// </summary>
        /// <param name="Properties">Initializes the object with the properties specified by <paramref name="Properties"/>.</param>
        /// <returns>Error.</returns>
        /// <seealso cref="ISystem::Initialize"/>
        virtual Error Initialize(std::vector<Properties::Property> Properties);

        /// <summary cref="GraphicObject::GetDesiredSystemChanges">
        ///   Implementation of the <c>IGeometryObject::GetDesiredSystemChanges</c> function.
        /// </summary>
        /// <returns>System::Types::BitMask - System changes desired by the object.</returns>
        /// <seealso cref="ISystemObject::GetSystemType"/>
        virtual System::Types::BitMask GetDesiredSystemChanges(void);

        /// <summary cref="GraphicObject::ChangeOccurred">
        ///   Implementation of the <c>IObserver::ChangeOccurred</c> function.
        /// </summary>
        /// <param name="pSubject">Subject of this notification.</param>
        /// <param name="ChangeType">Type of notification for this object.</param>
        /// <returns>Error.</returns>
        /// <seealso cref="IObserver::ChangeOccurred"/>
        virtual Error ChangeOccurred(ISubject* pSubject, System::Changes::BitMask ChangeType);

        /// <summary cref="GraphicObject::GetPotentialSystemChanges">
        ///   Implementation of the <c>ISubject::GetPotentialSystemChanges</c> function.
        /// </summary>
        /// <returns>System::Changes::BitMask - Returns systems changes possible for this object.</returns>
        /// <seealso cref="ISubject::GetPotentialSystemChanges"/>
        virtual System::Changes::BitMask GetPotentialSystemChanges(void);


    protected:

        Types                               m_Type;
        static pcstr                        sm_kapszTypeNames[];

        pcstr                               m_pszName;

        Ogre::SceneNode*                    m_pNode;

        DEFINE_SPIN_MUTEX(m_mutex);
};

