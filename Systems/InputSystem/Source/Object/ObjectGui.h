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


class InputObject;
class InputSystem;
class InputScene;
class InputTask;


class InputGuiObject : public InputObject {

    public:

        InputGuiObject(ISystemScene* pSystemScene, const char* pszName);
        ~InputGuiObject(void);

        /////////////////////////////////
        /// ISystemObject overrides
        /////////////////////////////////

        virtual Error Initialize(std::vector<Properties::Property> Properties);

        virtual void GetProperties(Properties::Array& Properties);

        virtual void SetProperties(Properties::Array Properties);

        virtual System::Types::BitMask GetDesiredSystemChanges(void);

        virtual void Update(f32 DeltaTime);

        /////////////////////////////////
        /// IObserver overrides
        /////////////////////////////////

        virtual Error ChangeOccurred(ISubject* pSubject, System::Changes::BitMask ChangeType);

        virtual System::Changes::BitMask GetPotentialSystemChanges(void);

    public:

        enum CommonPropertyTypes {
            Property_None, Property_Count
        };
        static const char*                        sm_kapszCommonPropertyNames[];
        static const Properties::Property   sm_kaCommonDefaultProperties[];

};

