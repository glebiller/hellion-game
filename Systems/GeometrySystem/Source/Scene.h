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


class GeometrySystem;
class GeometryObject;


class GeometryScene : public ISystemScene {
        friend GeometrySystem;

    protected:

        GeometryScene(ISystem* pSystem);

        virtual ~GeometryScene(void);

        virtual const void* GetSystemChangeData(System::Change SystemChange);

        /////////////////////////////////
        /// ISystemScene overrides
        /////////////////////////////////

        virtual System::Type GetSystemType(void);

        virtual Error Initialize(std::vector<Properties::Property> Properties);

        virtual void GetProperties(Properties::Array& Properties);

        virtual void SetProperties(Properties::Array Properties);

        virtual const char** GetObjectTypes(void);

        virtual ISystemObject* CreateObject(const char* pszName, const char* pszType);

        virtual Error DestroyObject(ISystemObject* pSystemObject);

        virtual ISystemTask* GetSystemTask(void);

        virtual void Update(f32 DeltaTime);

        /////////////////////////////////
        /// ISubject overrides
        /////////////////////////////////

        virtual System::Changes::BitMask GetPotentialSystemChanges(void);

    protected:

        std::list<GeometryObject*>          m_Objects;

};
