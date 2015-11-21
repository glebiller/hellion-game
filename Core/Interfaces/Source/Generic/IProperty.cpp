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

#include "Generic/IProperty.h"

#include <flatbuffers/flatbuffers.h>
#include <boost/lexical_cast.hpp>
#include <sstream>
#include <utility>

#include "Assert.h"
#include "Errors.h"
#include "MathUtils.h"
#include "Common_generated.h"
#include "System/Changes.h"

/**
 * @inheritDoc
 */
IProperty::IProperty()
    : m_bInitialized(false)
    , m_modified(0) {
    
};

/**
 * @inheritDoc
 */
IProperty::~IProperty() {
    
};

/**
 * @inheritDoc
 */
Error IProperty::initialize() {
    ASSERT(!m_bInitialized);
    m_bInitialized = true;
    return Errors::Success;
};

/**
 * @inheritDoc
 */
void IProperty::propertyChanged(System::Changes::BitMask uInChangedBits) {
    (void) uInChangedBits;
};
