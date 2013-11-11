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

#include "Debugger/ObjectChangesDebugger.h"

#ifdef DEBUG_BUILD

#include "Proto/Debug/Debug.pb.h"
#include "Proto/Debug/DebugEntity.pb.h"

#include "Debugger/Debugger.h"
#include "Universal/UScene.h"

ObjectChangesDebugger::ObjectChangesDebugger(Debugger* debugger)
    : m_pDebugger(debugger) {
}


ObjectChangesDebugger::~ObjectChangesDebugger(void) {
}

Error ObjectChangesDebugger::ChangeOccurred(ISubject* pSubject, System::Changes::BitMask ChangeType) {
    m_pDebugger->addUpdatedObject(pSubject);
    return Errors::Success;
}

#endif
