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


class NetworkScene;


///////////////////////////////////////////////////////////////////////////////
/// <summary>
///   <c>NetworkTask</c> Implementation of the ISystemTask interface for Network.
/// </summary>
///////////////////////////////////////////////////////////////////////////////

class NetworkTask : public ISystemTask {
        friend class NetworkScene;


    protected:

        NetworkTask(NetworkScene* pScene);
        ~NetworkTask(void);

        /// <summary cref="NetworkTask::GetSystemType">
        ///   Implementation of the <c>ISystemTask::GetSystemType</c> function.
        ///   Gets the system type for this system task.
        /// </summary>
        /// <returns>System::Type - The type of the system.</returns>
        /// <seealso cref="ISystemTask::GetSystemType"/>
        virtual System::Type GetSystemType(void);

        /// <summary cref="NetworkTask::Update">
        ///   Implementation of the <c>ISystemTask::Update</c> function.
        ///   Function informing the task to perform its updates.  This does
        ///   all the work needed to update Network objects for this frame.
        /// </summary>
        /// <param name="DeltaTime">The time delta from the last call.</param>
        /// <seealso cref="ISystemTask::Update"/>
        virtual void Update(f32 DeltaTime);

        /// <summary cref="NetworkTask::IsPrimaryThreadOnly">
        ///   Implementation of the <c>ISystemTask::IsPrimaryThreadOnly</c> function.
        /// </summary>
        /// <returns>bool - Returns true if this system should only run on the primary thread.</returns>
        /// <seealso cref="ISystemTask::IsPrimaryThreadOnly"/>
        virtual bool IsPrimaryThreadOnly(void) { return true; }


    private:

        NetworkScene*                     m_pScene;
};

