// Copyright 2008-2009 Intel Corporation
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

#include <System/Types.h>
#include "Object/Object.h"

class GraphicSystem;
class ISystemScene;

class SkyGraphicObject : public GraphicObject {
public:
    ///
    /// Constructor.
    ///
    /// @param [in,out] pSystemScene    If non-null, the system scene.
    /// @param  pszName                 The name.
    ///
    SkyGraphicObject(ISystemScene* pSystemScene, UObject* entity,
                     const Schema::SystemComponent& component);

    ///
    /// Destructor.
    ///
    ~SkyGraphicObject();
        
    ///
    /// @inheritDoc.
    ///
    void Update(f32 DeltaTime);
            
    ///
    /// @inheritDoc.
    ///
    System::Changes::BitMask GetPotentialSystemChanges() {
        return System::Changes::None;
    };
    
    ///
    /// @inheritDoc.
    ///
    System::Types::BitMask GetDesiredSystemChanges() {
        return System::Changes::None;
    };
    
    ///
    /// @inheritDoc.
    ///
    Error ChangeOccurred(ISubject* pSubject, System::Changes::BitMask ChangeType);

private:
    bool mSkysImported;

};