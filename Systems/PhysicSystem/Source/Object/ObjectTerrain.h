#pragma once

class HavokPhysicsSystem;
class HavokPhysicsScene;
class HavokPhysicsTask;

class PhysicObjectTerrain : public HavokObject {
        friend HavokPhysicsSystem;
        friend HavokPhysicsScene;
        friend HavokPhysicsTask;

    protected:

        PhysicObjectTerrain(ISystemScene* pSystemScene, const char* pszName);
        ~PhysicObjectTerrain(void);

        /// <summary cref="GraphicObjectCamera::Initialize">
        ///   Implementation of the <c>ISystem::Initialize</c> function.
        /// </summary>
        /// <param name="Properties">Initializes the object with the properties specified by <paramref name="Properties"/>.</param>
        /// <returns>Error.</returns>
        /// <seealso cref="GraphicObject::Initialize"/>
        virtual Error Initialize(std::vector<Properties::Property> Properties);

        /// <summary cref="GraphicObjectCamera::GetProperties">
        ///   Implementation of the <c>ISystem::GetProperties</c> function.
        /// </summary>
        /// <param name="Properties">Gets the properties of the object</param>
        /// <seealso cref="ISystem::GetProperties"/>
        virtual void GetProperties(Properties::Array& Properties);

        /// <summary cref="GraphicObjectCamera::SetProperties">
        ///   Implementation of the <c>ISystem::SetProperties</c> function.
        /// </summary>
        /// <param name="Properties">Sets the properties of the object</param>
        /// <seealso cref="ISystem::SetProperties"/>
        virtual void SetProperties(Properties::Array Properties);

        /// <summary cref="HavokCharacterObject::GetDesiredSystemChanges">
        ///   Implementation of the <c>IGeometryObject::GetDesiredSystemChanges</c> function.
        /// </summary>
        /// <returns>System::Types::BitMask - System changes desired by the Character object.</returns>
        /// <seealso cref="ISystemObject::GetSystemType"/>
        virtual System::Types::BitMask GetDesiredSystemChanges(void);

        /// <summary cref="GraphicObjectCamera::Update">
        ///   Implementation of the <c>ISystemTask::Update</c> method.
        /// </summary>
        /// <param name="DeltaTime">Elapsed time since the last frame.</param>
        /// <seealso cref="GraphicObject::Update"/>
        void Update(f32 DeltaTime);

        /////////////////////////////////
        /// IObserver overrides
        /////////////////////////////////

        /// <summary cref="HavokCharacterObject::ChangeOccurred">
        ///   Implementation of the <c>IObserver::ChangeOccurred</c> function.
        /// </summary>
        /// <param name="pSubject">Subject of this notification.</param>
        /// <param name="ChangeType">Type of notification for this object.</param>
        /// <returns>Error - any error codes</returns>
        /// <seealso cref="IObserver::ChangeOccurred"/>
        virtual Error ChangeOccurred(ISubject* pSubject, System::Changes::BitMask ChangeType);


        /// <summary cref="HavokCharacterObject::GetPotentialSystemChanges">
        ///   Implementation of the <c>ISubject::GetPotentialSystemChanges</c> function.
        /// </summary>
        /// <returns>System::Changes::BitMask - Returns systems changes possible for this Object.</returns>
        /// <seealso cref="ISubject::GetPotentialSystemChanges"/>
        virtual System::Changes::BitMask GetPotentialSystemChanges(void);

    private:

    public:

        enum CommonPropertyTypes {
            Property_Size ,/* Property_CapsuleB, Property_Radius,*/ Property_Count
        };
        static const char*                        sm_kapszCommonPropertyNames[];
        static const Properties::Property   sm_kaCommonDefaultProperties[];

};