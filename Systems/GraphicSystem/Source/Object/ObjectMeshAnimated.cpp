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
#include "ObjectMeshAnimated.h"
//#include "BehaviorAnimations.h"
//#include  "..\SystemAnimation\AnimationObject.h"


// Defines
#define POGRESCENEMGR (reinterpret_cast<OGREGraphicsScene*>(m_pSystemScene)->GetOGRESceneManager())
#define TRANSITION_TIME 2.0f

// Local data
pcstr GraphicObjectMeshAnimated::sm_kapszAnimatedPropertyNames[] = {
    "Skeleton", "Animation"
};

const Properties::Property GraphicObjectMeshAnimated::sm_kaAnimatedDefaultProperties[] = {
    Properties::Property(sm_kapszAnimatedPropertyNames[ Property_Skeleton ],
    VALUE1(Properties::Values::String),
    Properties::Flags::Valid,
    ""),
    Properties::Property(sm_kapszAnimatedPropertyNames[ Property_Animation ],
    VALUE2(Properties::Values::String, Properties::Values::String),
    Properties::Flags::Valid,
    "", ""),
};


///////////////////////////////////////////////////////////////////////////////
// GraphicObjectMesh - Constructor
GraphicObjectMeshAnimated::GraphicObjectMeshAnimated(
    ISystemScene* pSystemScene,
    pcstr pszName
)
    : GraphicObjectMesh(pSystemScene, pszName) {
    // Set default values
    m_Velocity = Math::Vector3::Zero;
    m_MaxVelocity = 0.0f;
    m_Type = GraphicObject::Type_MeshAnimated;
}


///////////////////////////////////////////////////////////////////////////////
// GraphicObjectMesh - Destructor
GraphicObjectMeshAnimated::~GraphicObjectMeshAnimated(
    void
) {
    // Clear animations
    std::list<Animation*>::iterator it = m_Animations.begin();

    for (it = m_Animations.begin(); it != m_Animations.end(); it++) {
        delete *it;
    }
}


///////////////////////////////////////////////////////////////////////////////
// Update - Update this Object (updates animation)
void
GraphicObjectMeshAnimated::Update(
    f32 DeltaTime
) {
    // Determine scale value based on velocity and y direction (for flying stuff)
    f32 Scale = 1.0f;

    if (m_MaxVelocity) {
        Math::Vector3 vVelocityNormal(m_Velocity);
        vVelocityNormal.Normalize();
        Scale = Math::Min(1.0f, m_Velocity.Magnitude() / m_MaxVelocity);

        if (vVelocityNormal.y > 0.5f) {
            Scale *= (1.0f + (8.0f * (vVelocityNormal.y - 0.5f)));
        } else if (vVelocityNormal.y < -0.5f) {
            Scale = 0.0f;
        }
    }

    // IDF Hack to support windmill
    if (strcmp(m_pszName, "windmill") == 0) {
        Scale = 1.0f;
        // Enable all animations for the windmill
        std::list<Animation*>::iterator it = m_Animations.begin();

        for (it = m_Animations.begin(); it != m_Animations.end(); it++) {
            (*it)->m_State->setEnabled(True);
        }
    }

    // Update all animations
    std::list<Animation*>::iterator it = m_Animations.begin();

    for (it = m_Animations.begin(); it != m_Animations.end(); it++) {
        // Add time to the animation
        Animation* pAnimation = *it;
        pAnimation->m_State->addTime(DeltaTime * Scale);

        // Update the transition (if needed)
        if (pAnimation->m_Transition != 0.0f) {
            f32 Weight = (TRANSITION_TIME - pAnimation->m_Transition) / TRANSITION_TIME;

            if (pAnimation->m_State->getEnabled() && pAnimation->m_Disable) {
                Weight = 1.0f - Weight;
            }

            // Set the weight for the transition
            pAnimation->m_State->setWeight(Weight);
            // Adjust amount of time in transition
            pAnimation->m_Transition = Math::Max(0.0f, pAnimation->m_Transition - DeltaTime);

            if (pAnimation->m_Transition == 0.0f && pAnimation->m_Disable) {
                pAnimation->m_State->setEnabled(False);
            }
        }
    }

    // Process animations
    //m_pEntity->_updateAnimation();
}


///////////////////////////////////////////////////////////////////////////////
// GraphicObjectMesh - Destructor
Error
GraphicObjectMeshAnimated::Initialize(
    std::vector<Properties::Property> Properties
) {
    Error Err = Errors::Failure;
    ASSERT(!m_bInitialized);
    // Call the base class
    GraphicObjectMesh::Initialize(Properties);
    // Move animation to the CPU
    m_pEntity->addSoftwareAnimationRequest(True);
    return Err;
}


///////////////////////////////////////////////////////////////////////////////
// GetProperties - Properties for this Object are returned in Properties
void
GraphicObjectMeshAnimated::GetProperties(
    Properties::Array& Properties
) {
    //
    // Get the base class properties first.
    //
    GraphicObjectMesh::GetProperties(Properties);
    //
    // Add all the properties.
    //
    Properties.reserve(Properties.size() + Property_Count);

    for (i32 i = 0; i < Property_Count; i++) {
        Properties.push_back(sm_kaAnimatedDefaultProperties[ i ]);
    }
}



///////////////////////////////////////////////////////////////////////////////
// SetProperties - Set properties for this Object
void
GraphicObjectMeshAnimated::SetProperties(
    Properties::Array Properties
) {
    ASSERT(m_bInitialized);

    //
    // Read in the properties.
    //
    for (Properties::Iterator it = Properties.begin(); it != Properties.end(); it++) {
        if (it->GetFlags() & Properties::Flags::Valid) {
            std::string sName = it->GetName();

            if (sName == sm_kapszAnimatedPropertyNames[ Property_Skeleton ]) {
                //
                // Load a skeleton
                //
                if (m_pEntity != NULL) {
                    m_pEntity->getMesh()->setSkeletonName(it->GetString(0));
                }
            } else if (sName == sm_kapszAnimatedPropertyNames[ Property_Animation ]) {
                //
                // Create a new animation node
                //
                if (m_pEntity != NULL) {
                    Animation* pAnimation = new Animation();
                    // Init animation
                    pAnimation->m_Transition = 0.0f;
                    pAnimation->m_Disable = False;
                    // Load animation state
                    std::string AnimationName = it->GetString(0);
                    pAnimation->m_State = m_pEntity->getAnimationState(AnimationName);
                    ASSERT(pAnimation->m_State);
                    // Set associated behavior
                    std::string BehaviorName = it->GetString(1);

                    if (BehaviorName == "Idle" || BehaviorName == "idle") {
                        pAnimation->m_Behavior = e_Behavior_Idle;
                    } else if (BehaviorName == "Walking" || BehaviorName == "walking") {
                        pAnimation->m_Behavior = Interface::e_Behavior_Walking;
                    } else if (BehaviorName == "Running" || BehaviorName == "running") {
                        pAnimation->m_Behavior = Interface::e_Behavior_Running;
                    } else if (BehaviorName == "Flying" || BehaviorName == "flying") {
                        pAnimation->m_Behavior = Interface::e_Behavior_Flying;
                    } else if (BehaviorName == "Panicked" || BehaviorName == "panicked") {
                        pAnimation->m_Behavior = Interface::e_Behavior_Panicked;
                    } else {
                        pAnimation->m_Behavior = Interface::e_Behavior_None;
                    }

                    // Store the animation
                    m_Animations.push_back(pAnimation);
                }
            }

            //
            // Set this property to invalid since it's already been read.
            //
            it->ClearFlag(Properties::Flags::Valid);
        }
    }
}


///////////////////////////////////////////////////////////////////////////////
// GetDesiredSystemChanges - Returns the systems changes this Object
//                           is intrested in hearing about
System::Types::BitMask
GraphicObjectMeshAnimated::GetDesiredSystemChanges(
    void
) {
    return (System::Changes::Input::Velocity
            | GraphicObjectMesh::GetDesiredSystemChanges());
}


///////////////////////////////////////////////////////////////////////////////
// ChangeOccurred - Notify this Object a change occureed (change handler)
Error
GraphicObjectMeshAnimated::ChangeOccurred(
    ISubject* pSubject,
    System::Changes::BitMask ChangeType
) {
    // Handle animation changes
    /*if ( ChangeType & System::Changes::AI::Behavior )
    {
        IBehaviorObject* pBehaviorObject = dynamic_cast<IBehaviorObject*>(pSubject);
        ASSERT( pBehaviorObject );

        // Enable any animations associated with this behavior
        std::list<Animation*>::iterator it = m_Animations.begin();
        for ( it = m_Animations.begin(); it != m_Animations.end(); it++ )
        {
            Animation* pAnimation = *it;
            if( pAnimation->m_Behavior == pBehaviorObject->GetBehavior() )
            {
                pAnimation->m_State->setEnabled( True );

                pAnimation->m_Disable = False;
                pAnimation->m_Transition = TRANSITION_TIME;
            }
            else
            {
                if( pAnimation->m_State->getEnabled() )
                {
                    pAnimation->m_Disable = True;
                    pAnimation->m_Transition = TRANSITION_TIME;
                }
            }
        }
    }*/
    /*if ( ChangeType & System::Changes::AI::Velocity )
    {
        // Store the velocity data
        m_Velocity = *dynamic_cast<IMoveObject*>(pSubject)->GetVelocity();
        m_MaxVelocity = dynamic_cast<IMoveObject*>(pSubject)->GetMaxVelocity();
    }*/
    // Let the parent class handle the rest
    return GraphicObjectMesh::ChangeOccurred(pSubject, ChangeType);
}


///////////////////////////////////////////////////////////////////////////////
// GetPotentialSystemChanges - Returns systems changes possible for this Object
System::Changes::BitMask
GraphicObjectMeshAnimated::GetPotentialSystemChanges(
    void
) {
    return System::Changes::Graphics::Animation | GraphicObjectMesh::GetPotentialSystemChanges();
}

