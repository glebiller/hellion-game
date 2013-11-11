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

#pragma once

#include "Defines.h"

#if defined DEBUG_BUILD

#include <boost/timer/timer.hpp>

#include "Generic/IObserver.h"

class Debugger;

/**
 * Object changes debugger.
 *
 * @sa  IObserver
 */
class ObjectChangesDebugger : public IObserver {
public:

    /**
     * Constructor.
     *
     * @param [in,out]  debugger    If non-null, the debugger.
     */
    ObjectChangesDebugger(Debugger* debugger);

    /**
     * Destructor.
     */
    ~ObjectChangesDebugger(void);
    
    /**
     * @inheritDoc
     */
    Error ChangeOccurred(ISubject* pSubject, System::Changes::BitMask ChangeType);
    
    /**
     * @inheritDoc
     */
    inline System::Changes::BitMask GetDesiredSystemChanges(void) {
        return System::Changes::All;
    }

private:
    Debugger*                       m_pDebugger;

};

#endif
