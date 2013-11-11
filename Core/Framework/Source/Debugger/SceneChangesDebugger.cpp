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

#include "Debugger/SceneChangesDebugger.h"

#if defined DEBUG_BUILD

#include "Proto/Debug/Debug.pb.h"
#include "Proto/Debug/DebugEntity.pb.h"

#include "Debugger/Debugger.h"
#include "Object/ISceneObject.h"
#include "Universal/UScene.h"

SceneChangesDebugger::SceneChangesDebugger(Debugger* debugger) :
    m_pDebugger(debugger) {
}


SceneChangesDebugger::~SceneChangesDebugger(void) {
}

Error SceneChangesDebugger::ChangeOccurred(ISubject* pSubject, System::Changes::BitMask ChangeType) {
    if (ChangeType & System::Changes::Generic::CreateObject) {
        for (auto objectProto : *dynamic_cast<ISceneObject*>(pSubject)->getCreateObjects()) {
            m_pDebugger->addCreatedObjectIds(objectProto.name());
        }
    }
    return Errors::Success;
}

#endif