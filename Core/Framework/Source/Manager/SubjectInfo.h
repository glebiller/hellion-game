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

class ISubject;
class ObserversList;

#include <vector>

#include "DataTypes.h"
#include "Manager/ObserverRequest.h"

///
/// Represents a list of observers extended with cumulative data.
///
struct SubjectInfo {
    typedef std::vector<ObserverRequest> ObserversList;

    SubjectInfo()
        : m_pSubject(nullptr)
        , m_interestBits(0) {
    }

    ISubject*       m_pSubject;
    u32             m_interestBits;
    ObserversList       m_observersList;
};