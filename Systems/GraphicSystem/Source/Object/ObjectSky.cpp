//
// extern includes
//
#pragma warning( push, 0 )
#pragma warning( disable : 6326 6385 )
#include "Ogre.h"
#pragma warning( pop )

//
// Core includes
//
#include "BaseTypes.h"
#include "Interface.h"

//
// Graphic system includes
//
#include "System.h"
#include "Scene.h"
#include "Object.h"
#include "ObjectSky.h"

#define POGRESCENEMGR (reinterpret_cast<OGREGraphicsScene*>(m_pSystemScene)->GetOGRESceneManager())


GraphicObjectSky::GraphicObjectSky(
    ISystemScene* pSystemScene,
    const char* pszName
)
    : GraphicObject(pSystemScene, pszName),
      mSkysImported(false) {
    m_Type = GraphicObject::Type_Sky;
}

GraphicObjectSky::~GraphicObjectSky(
    void
) {
}

Error
GraphicObjectSky::Initialize(
    std::vector<Properties::Property> Properties
) {
    Error Err = Errors::Success;
    ASSERT(!m_bInitialized);
    //
    // Call the base class.
    //
    GraphicObject::Initialize(Properties);
    return Err;
}

void
GraphicObjectSky::SetProperties(
    Properties::Array Properties
) {
    ASSERT(m_bInitialized);
}

void
GraphicObjectSky::GetProperties(
    Properties::Array& Properties
) {
}

void
GraphicObjectSky::Update(f32 DeltaTime) {
    UNREFERENCED_PARAM(DeltaTime);
}