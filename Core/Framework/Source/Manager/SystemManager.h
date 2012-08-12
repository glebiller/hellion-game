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

#include "Proto/Common/System.pb.h"

#include "Errors.h"
#include "Singleton.h"
#include "System.h"

class ISystem;

/**
 * Provides a single location for access to the different systems that are available for use.
 * @sa  Singleton
 */
class SystemManager : public Singleton {
    public:

        /**
         * Constructor.
         */
        SystemManager(void);

        /**
         * Destructor.
         */
        ~SystemManager(void);

        /**
         * Adds a new system to the collection.  Called by the ISystem constructor.
         *
         * @param   pSystem A pointer to the system to add.
         * @return  An error code.
         */
        Error Add(ISystem* pSystem);

        /**
         * Removes a system from the collection.  Called by the ISystem destructor.
         *
         * @param   SystemType  The system type to remove.
         * @return  An error code.
         */
        Error Remove(const SystemProto::Type SystemType);

        /**
         * Gets a specific system from the collection based on its type.
         *
         * @param   SystemType  The type of system to get.
         * @return  A pointer to the system.
         */
        ISystem* Get(const SystemProto::Type SystemType);

        /**
         * Gets a specific system from the collection base on its name.
         *
         * @param   pszName The name of system to get.
         * @return  A pointer to the system or <c>NULL</c> of failure.
         */
        ISystem* Get(const char* pszName);

        //
        // TODO: These need to be defined better.  Maybe use something like the stl
        //  iterator. The owner of the Scheduler class needs to take care of this
        //  since it will be the primary consumer.
        //
        ISystem* GetFirst(void);
        ISystem* GetNext(void);


    protected:

        std::map<SystemProto::Type, ISystem*>              m_Systems;
        std::map<SystemProto::Type, ISystem*>::iterator    m_SystemIt;

};



DeclareSingleton(SystemManager);
