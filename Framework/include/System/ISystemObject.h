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

#include <list>
#include <boost/uuid/uuid.hpp>
#include <schema/scene_generated.h>
#include <schema/system_type_generated.h>
#include <Manager/ObserverRequest.h>
#include "Generic/IObserver.h"
#include "System/Changes.h"
#include "Types.h"

class ISystemScene;

class UObject;

/**
 * <c>ISystemObject</c> is an interface class designed to be an extension of the UObject.
 * Systems can extend the UObject by implementing this interface to give it new  properties.
 * An example would be a physics system implementing this interface so that the UObject would
 * now be able to interact with the physics system.
 *
 * @sa  CSubject
 * @sa  IObserver
 */
// TODO rename SystemComponent
class ISystemObject : public IObserver {
public:

    static const unsigned int InvalidObserverID = (const unsigned int) -1;

    /**
     * @inheritDoc
     */
    ISystemObject(ISystemScene* pSystemScene, UObject* entity, const Schema::SystemComponent& component);

    /**
     * @inheritDoc
     */
    virtual ~ISystemObject();

    unsigned int getObserverId(IObserver* pObserver) const;

    /**
     * Associates the provided IObserver with the given ISystemObject aspects of interest.
     * This method is typically called from @e ChangeManager::Register()
     *  or the IObserver, if used without a ChangeManager.
     *
     * @param   pInObserver     A pointer to the IObserver.
     * @param   uInIntrestBits  The aspects of interest that changed as defined by the supplied
     *                          ISystemObject's published interest bits.
     * @param   uID             ID assigned by pInObserver to this subject.
     * @param   shiftBits       Used for components supporting multiply inherited interfaces each
     *                          with subject interfaces.
     */
    void Attach(IObserver* pInObserver, System::Types::BitMask uInIntrestBits, unsigned int uID);

    /**
     * Disassociates the provided Observer with the Subject.This method is typically called from @e ChangeManager::Register()
     *  or the IObserver, if used without a ChangeManager.
     *
     * @param   pInObserver A pointer to the IObserver that should be disassociated.
     * @return  One of the following Error codes: Error::Success No error. Error::InvalidAddress
     *          pInObserver and/or pInSubject was NULL.
     */
    void Detach(IObserver* pInObserver);

    /**
     * Updates the interest bits.
     *
     * @param [in,out]  pInObserver If non-null, the in observer.
     * @param   uInIntrestBits      The in intrest bits.
     * @return  The Error code. Error::Success No error.
     */
    virtual void UpdateInterestBits(IObserver* pInObserver, unsigned int uInIntrestBits);

    /**
     * Publishes to attached Observers and ChanageManager that changes have occurred.
     * This method is typically called from @e ChangeManager::Register()
     * or the IObserver, if used without a ChangeManager.
     *
     * @param   uInChangedBits  The unsigned int bit field that describes the conceptual change with respect
     *                          to the published interests.
     */
    void PostChanges(System::Changes::BitMask uInChangedBits);

    /**
     * Identifies the system changes that this subject could possibly make.
     *
     * @return  A bitmask of the possible system changes.
     */
    virtual System::Changes::BitMask GetPotentialSystemChanges() = 0;

    virtual void Update(float DeltaTime) = 0;

    virtual inline const void* getComponentData() {
        return component_.data();
    }

    inline void * getMutableComponentData() {
        return const_cast<void*>(getComponentData());
    }

    template <typename ComponentDataType>
    inline const ComponentDataType* getComponent() {
        return static_cast<const ComponentDataType*>(getComponentData());
    }

    template<typename ComponentDataType>
    inline ComponentDataType* getMutableComponent() {
        return static_cast<ComponentDataType*>(getMutableComponentData());
    }

    inline Schema::SystemType GetSystemType() {
        return component_.systemType();
    }

    inline Schema::ComponentType getComponentType() {
        return component_.data_type();
    }

    /**
     * Gets the system scene this object belongs to.
     *
     * @return  A pointer to the system.
     */
    template<typename TSystemScene>
    inline TSystemScene* GetSystemScene() {
        return static_cast<TSystemScene*>(m_pSystemScene);
    }

    inline UObject* getEntity() {
        return m_entity;
    }

protected:
    const Schema::SystemComponent& component_;
    // IMPLEMENTATION NOTE
    // Since only Change Control Managers (CCM) are supposed to subscribe for
    // notifications sent by PostChange, there are not many observers expected
    // to be in the list. Another assumption is that repeated attaches are infrequent.
    // Thus the most frequent operation is the traversal and thus the usual std::list
    // will suit fine here.
    std::list<ObserverRequest> observers_;
    ISystemScene* m_pSystemScene;
    UObject* m_entity;


private:

    /**
     * Performs an Atomic Compare and Swap.
     *
     * @param [in,out]  interestBits  The value to compare and write to.
     * @param   newBits          The new value to set if needed.
     * @param   prevBits         The old value to compare to.
     * @return  The new value if it was written to or original value.
     */
    long AtomicCompareAndSwap(unsigned int interestBits, long newBits, long prevBits);

};