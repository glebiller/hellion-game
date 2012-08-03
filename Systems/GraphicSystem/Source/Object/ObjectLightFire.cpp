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


//
// extern includes
//
#pragma warning( push, 0 )
// Temporarily switching warning level to 0 to ignore warnings in extern/Ogre
#include "Ogre.h"
#pragma warning( pop )

#ifdef SAFE_DELETE
#undef SAFE_DELETE
#endif
#ifdef SAFE_DELETE_ARRAY
#undef SAFE_DELETE_ARRAY
#endif

//
// core includes
//
#include <BaseTypes.h>
#include <Interface.h>

//
// Ogre system includes
//
#include "Scene.h"
#include "System.h"
#include "Object.h"
#include "ObjectLightFire.h"


#define POGRESCENEMGR (reinterpret_cast<OGREGraphicsScene*>(m_pSystemScene)->GetOGRESceneManager())


pcstr GraphicObjectLightFire::sm_kapszPropertyNames[] = {
    "Flare", "Flicker"
};

const Properties::Property GraphicObjectLightFire::sm_kaDefaultProperties[] = {
    Properties::Property(sm_kapszPropertyNames[ Property_Flare ],
    Properties::Values::Color3,
    Properties::Flags::Valid,
    "R", "G", "B", NULL,
    Math::Color3::Black),
    Properties::Property(sm_kapszPropertyNames[ Property_Flicker ],
    Properties::Values::Float32,
    Properties::Flags::Valid,
    NULL, NULL, NULL, NULL,
    0.0f),
};


///////////////////////////////////////////////////////////////////////////////
// GraphicObjectLightFire - Constructor
GraphicObjectLightFire::GraphicObjectLightFire(
    ISystemScene* pSystemScene,
    pcstr pszName
)
    : GraphicObjectLight(pSystemScene, pszName) {
    ASSERT(Property_Count == sizeof sm_kapszPropertyNames / sizeof sm_kapszPropertyNames[ 0 ]);
    ASSERT(Property_Count == sizeof sm_kaDefaultProperties / sizeof sm_kaDefaultProperties[ 0 ]);
    m_Type = GraphicObject::Type_LightFire;
    m_LightType = LightType_Point;
    m_BaseColor = Ogre::ColourValue();
    m_BaseFlare = Ogre::ColourValue();
    m_Flicker   = 0.0f;
    m_Flare     = Ogre::ColourValue(0.1f, 0.1f, 0.1f);
    m_TotalTime = 0.0f;
}


///////////////////////////////////////////////////////////////////////////////
// ~GraphicObjectLightFire - Destructor
GraphicObjectLightFire::~GraphicObjectLightFire(
    void
) {
}


///////////////////////////////////////////////////////////////////////////////
// Update - Animate the light flickering
void GraphicObjectLightFire::Update(
    f32 DeltaTime
) {
    if (m_pLight != NULL) {
        if (m_Flicker > 0.0f && m_pLight->getVisible()) {
            // Increase total time based on flicker rate
            m_TotalTime += DeltaTime * m_Flicker;
            // Determine the new color for this light
            Ogre::ColourValue Color = m_BaseColor;
            // Flicker level ( based on sin mapped to [0.0, 1.0f] )
            f32 Level = (Math::Angle::Sin(m_TotalTime) + 1.0f) * 0.5f;
            Color.r = m_BaseColor.r + m_Flare.r * Level;
            Color.g = m_BaseColor.g + m_Flare.g * Level;
            Color.b = m_BaseColor.b + m_Flare.b * Level;
            Color.saturate();
            m_pLight->setDiffuseColour(Color);

            // If the flicker level is near zero, pick a new random flare
            if (Level < 0.1f) {
                f32 Rand = ((f32)rand() / (f32)RAND_MAX);
                m_Flare.r = m_BaseFlare.r * Rand;
                m_Flare.g = m_BaseFlare.g * Rand;
                m_Flare.b = m_BaseFlare.b * Rand;
            }
        }
    }
}


///////////////////////////////////////////////////////////////////////////////
// Initialize - Initializes this Object with the given properties
Error
GraphicObjectLightFire::Initialize(
    std::vector<Properties::Property> Properties
) {
    Error Err = Errors::Failure;
    ASSERT(!m_bInitialized);
    //
    // Call the base class.
    //
    Err = GraphicObjectLight::Initialize(Properties);
    return Err;
}


///////////////////////////////////////////////////////////////////////////////
// GetProperties - Properties for this Object are returned in Properties
void
GraphicObjectLightFire::GetProperties(
    Properties::Array& Properties
) {
    //
    // Call base class
    //
    GraphicObjectLight::GetProperties(Properties);
    //
    // Get the index of our first item.
    //
    i32 iProperty = static_cast<i32>(Properties.size());
    //
    // Add all the properties.
    //
    Properties.reserve(Properties.size() + Property_Count);

    for (i32 i = 0; i < Property_Count; i++) {
        Properties.push_back(sm_kaDefaultProperties[ i ]);
    }

    //
    // Modify the default values.
    //
    Math::Color3 Flare = { m_BaseFlare.r, m_BaseFlare.g, m_BaseFlare.b };
    Properties[ iProperty + Property_Flare ].SetValue(Flare);
    Properties[ iProperty + Property_Flicker ].SetValue(0, m_Flicker);
}


///////////////////////////////////////////////////////////////////////////////
// SetProperties - Set properties for this Object
void
GraphicObjectLightFire::SetProperties(
    Properties::Array Properties
) {
    ASSERT(m_bInitialized);

    //
    // Read in the properties.
    //
    for (Properties::Iterator it = Properties.begin(); it != Properties.end(); it++) {
        if (it->GetFlags() & Properties::Flags::Valid) {
            std::string sName = it->GetName();

            if (sName == sm_kapszPropertyNames[ Property_Flare ]) {
                m_BaseFlare = Ogre::ColourValue(it->GetColor3().r, it->GetColor3().g, it->GetColor3().b);
                // Set this property to invalid since it's already been read
                it->ClearFlag(Properties::Flags::Valid);
            } else if (sName == sm_kapszPropertyNames[ Property_Flicker ]) {
                m_Flicker = it->GetFloat32(0);
                // Set this property to invalid since it's already been read
                it->ClearFlag(Properties::Flags::Valid);
            } else if (sName == GraphicObjectLight::sm_kapszPropertyNames[ Property_Diffuse ]) {
                // Store the original color (don't mark his property invalid because base class might use it)
                m_BaseColor.r = it->GetColor3().r;
                m_BaseColor.g = it->GetColor3().g;
                m_BaseColor.b = it->GetColor3().b;
            }
        }
    }

    //
    // Call base class.
    //
    GraphicObjectLight::SetProperties(Properties);

    //
    // Disable the light
    //
    if (m_pLight) {
        m_pLight->setVisible(False);
    }
}


///////////////////////////////////////////////////////////////////////////////
// GetDesiredSystemChanges - Returns the systems changes this Object
//                           is intrested in hearing about
System::Changes::BitMask
GraphicObjectLightFire::GetDesiredSystemChanges(
    void
) {
    return GraphicObjectLight::GetDesiredSystemChanges() /*| System::Changes::POI::Area*/;
}



///////////////////////////////////////////////////////////////////////////////
// ChangeOccurred - Notify this Object a change occureed (change handler)
Error
GraphicObjectLightFire::ChangeOccurred(
    ISubject* pSubject,
    System::Changes::BitMask ChangeType
) {
    ASSERT(m_bInitialized);
    /*if( ChangeType & System::Changes::POI::Area )
    {
        IAreaObject* pAreaObject = dynamic_cast<IAreaObject*>(pSubject);
        ASSERT( pAreaObject );

        // Get data
        Math::Vector3 Min, Max;
        pAreaObject->GetAreaBB( Min, Max );


        if( m_pLight )
        {
            // Set the position
            Math::Vector3 Center = ( Min + Max ) * 0.5f;
            m_pLight->setPosition( Center.x, Center.y, Center.z );

            // Enable the light if needed
            m_pLight->setVisible( pAreaObject->IsAreaActive() ? true : false );
        }
    }*/
    // Call the base class
    return GraphicObjectLight::ChangeOccurred(pSubject, ChangeType);
}

