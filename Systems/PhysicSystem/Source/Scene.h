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


#include <Physics/Collide/Shape/Compound/Collection/SimpleMesh/hkpSimpleMeshShape.h>

#include <SpinMutex.h>

class HavokPhysicsSystem;
class HavokPhysicsTask;
class HavokPhysicsObject;
class HavokCharacterObject;


class HavokPhysicsScene : public ISystemScene, public IGenericScene {
        friend HavokPhysicsSystem;
        friend HavokPhysicsTask;


    protected:

        HavokPhysicsScene(ISystem* pSystem);

        ~HavokPhysicsScene(void);

        /////////////////////////////////
        /// ISystemScene overrides
        /////////////////////////////////

        virtual System::Type GetSystemType(void);

        virtual void GlobalSceneStatusChanged(GlobalSceneStatus Status);

        virtual Error Initialize(std::vector<Properties::Property> Properties);

        virtual void GetProperties(std::vector<Properties::Property>& Properties);

        virtual void SetProperties(std::vector<Properties::Property> Properties);

        virtual pcstr* GetObjectTypes(void);

        virtual ISystemObject* CreateObject(pcstr pszName, pcstr pszType);

        virtual Error DestroyObject(ISystemObject* pSystemObject);

        virtual ISystemTask* GetSystemTask(void);

        virtual void Update(f32 DeltaTime);

        /////////////////////////////////
        /// ISubject overrides
        /////////////////////////////////

        virtual System::Changes::BitMask GetPotentialSystemChanges(void);

        /////////////////////////////////
        /// IGenericScene overrides
        /////////////////////////////////

        virtual void GetCreateObjects(CreateObjectDataArray& apszNames);

        virtual void GetDestroyObjects(std::vector<pcstr>& apszNames);

        virtual void GetExtendObjects(ExtendObjectDataArray& apszNames);

        virtual void GetUnextendObjects(std::vector<pcstr>& apszNames);

        virtual ISystemObject* ExtendObject(pcstr pszName, void* pUserData);

        virtual ISystemObject* UnextendObject(pcstr pszName);

    public:

        /// <summary cref="HavokPhysicsScene::GetWorld">
        ///   Gets the Havok World up direction.
        /// </summary>
        /// <returns>hkVector4 - A vector containing the up direction of Havok World.</returns>
        static hkVector4 GetDefaultUp(void) {
            return sm_kDefaultUp;
        }

        /// <summary cref="HavokPhysicsScene::GetWorld">
        ///   Gets the Havok World used by this scene.
        /// </summary>
        /// <returns>hkpWorld* - A pointer this scenes Havok World.</returns>
        hkpWorld* GetWorld(void) {
            ASSERT(m_pWorld != NULL);
            return m_pWorld;
        }

        /// <summary cref="HavokPhysicsScene::GetTask">
        ///   Gets the Task associated with this scene.
        /// </summary>
        /// <returns>HavokPhysicsTask* - A pointer this scenes Task.</returns>
        HavokPhysicsTask* GetTask(void) {
            return m_pTask;
        }

        //const Math::Vector3& GetGravity( void )
        //{
        //    return m_Gravity;
        //}

        /// <summary cref="HavokPhysicsScene::GetCharacters">
        ///   Gets the all Character objects in this scene.
        /// </summary>
        /// <returns>std::list - A list of Character objects in the scene.</returns>
        inline std::list<HavokCharacterObject*> GetCharacters(void) { return m_Characters; }

        /// <summary cref="HavokPhysicsScene::GetMaterialId">
        ///   Returns the ID for the given material name.
        /// </summary>
        /// <param name="sName">Material name.</param>
        /// <returns>i32 - Unique ID associated with the given name.</returns>
        i32 GetMaterialId(const std::string& sName);

        /// <summary cref="HavokPhysicsScene::AddTrackCollision">
        ///   Adds a collision relationship to track and report.
        /// </summary>
        /// <param name="pSubject">Subject that can cause collisions.</param>
        /// <param name="pObserver">Observer that wants to hear about collisions.</param>
        void AddTrackCollision(HavokPhysicsObject* pSubject, HavokPhysicsObject* pObserver);

        /// <summary cref="HavokPhysicsScene::RemoveTrackCollision">
        ///   Removes a collision relationship to track and report
        /// </summary>
        /// <param name="pSubject">Subject that could have cause collisions.</param>
        /// <param name="pObserver">Observer that wanted to hear about collisions.</param>
        void RemoveTrackCollision(HavokPhysicsObject* pSubject, HavokPhysicsObject* pObserver);

    protected:

        static const Math::Vector3          sm_kDefaultGravity;
        static const hkVector4              sm_kDefaultUp;

        const hkVector4                     m_kVelocityModifier;
        const f32                           m_kfMass;

        enum PropertyTypes {
            Property_SceneFile,
            Property_Gravity,
            Property_Material, Property_Elasticity, Property_Friction, Property_Softness,
            Property_Count
        };

        static pcstr                        sm_kapszPropertyNames[];
        static const Properties::Property   sm_kaDefaultProperties[];

        std::list<HavokCharacterObject*>    m_Characters;

        HavokPhysicsTask*                   m_pTask;

        hkpWorld*                           m_pWorld;
        Math::Vector3                       m_Gravity;

        struct Material {
            std::string                     Name;
            i32                             Id;
        };
        typedef std::vector<Material>       Materials;
        Materials                           m_Materials;

        void*                               m_pLoader;

        enum SceneFileTypes {
            Extend, Loose, BreakOffParts,
        };

        struct SceneFileData {
            std::string                     sPathName;
            SceneFileTypes                  Type;
            f32                             Value1;
            f32                             Value2;
        };
        typedef std::vector<SceneFileData>  SceneFiles;
        SceneFiles                          m_SceneFiles;

        struct ExtensionData {
            pcstr                           pszName;
            hkpRigidBody*                   pBody;
        };
        std::map<void*, ExtensionData>      m_aExtensions;

        hkArray<hkpRigidBody*>              m_aUnattachedBodies;

        DEFINE_SPIN_MUTEX(m_BrokenOffPartsSpinWait);

};

