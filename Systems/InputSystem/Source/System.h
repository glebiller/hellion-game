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


class InputSystem : public ISystem {
    public:

        InputSystem(void);

        virtual ~InputSystem(void);

    protected:

        /////////////////////////////////
        /// ISystem overrides
        /////////////////////////////////

        virtual pcstr GetName(void);

        virtual System::Type GetSystemType(void);

        virtual Error Initialize(Properties::Array Properties);

        virtual void GetProperties(Properties::Array& Properties);

        virtual void SetProperties(Properties::Array Properties);

        virtual ISystemScene* CreateScene(void);

        virtual Error DestroyScene(ISystemScene* pSystemScene);

    public:

        typedef struct _HotKey {
            std::string psKey;
            Bool  bCtrl;
        } HotKey;
        std::map<pcstr, HotKey*> m_aHotKeys;
        typedef std::map<pcstr, HotKey*>::iterator hotKeyIt;

    protected:

        enum HotKeyTypes {
            HotKey_MoveForward,
            HotKey_MoveBack,
            HotKey_MoveLeft,
            HotKey_MoveRight,
            HotKey_Quit,
            HotKey_PolygonMode,
            HotKey_Count
        };

        static pcstr sm_kapszPropertyNames[];
        static const Properties::Property sm_kaDefaultProperties[];

};

