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

#include "DataTypes.h"
#include "System/Changes.h"

class IObserver;

/**
 * Observer request.
 */
class ObserverRequest {
public:
    /**
     * Constructor.
     *
     * @param [in,out]  pObserver   (Optional) If non-null, (Optional) the observer.
     * @param   Interests           (Optional) the interests.
     * @param   idBits              (Optional) the identifier bits.
     */
    ObserverRequest(IObserver* pObserver = nullptr, u32 Interests = 0, u32 idBits = System::Changes::All)
        : m_pObserver(pObserver)
        , m_interestBits(Interests)
        , m_observerIdBits(idBits) {
    }

    /**
     * Less-than comparison operator.
     *
     * @param   rhs The right hand side.
     *
     * @return  true if the first parameter is less than the second.
     */
    bool operator < (const ObserverRequest& rhs) const {
        return m_pObserver < rhs.m_pObserver;
    }

    /**
     * Equality operator.
     *
     * @param [in,out]  rhs If non-null, the right hand side.
     *
     * @return  true if the parameters are considered equivalent.
     */
    bool operator == (IObserver* rhs) const {
        return m_pObserver == rhs;
    }

    IObserver*  m_pObserver;
    u32         m_interestBits;
    u32         m_observerIdBits;
};
