
#include "Object/PhysicDebugGraphicObject.h"

#include "GraphicScene.h"

PhysicDebugGraphicObject::PhysicDebugGraphicObject(ISystemScene& pSystemScene, UObject& entity,
                                                   const Schema::SystemComponent& component)
        : ISystemObject(&pSystemScene, &entity, component) {
    auto sceneManager = GetSystemScene<GraphicScene>()->getSceneManager();
    Ogre::ManualObject* manual = sceneManager->createManualObject();

    manual->begin("BaseWhiteNoLighting", Ogre::RenderOperation::OT_LINE_LIST);
    manual->position(-100, -100, -100);
    manual->position(100, 100, 100);
    manual->end();
    sceneManager->getRootSceneNode()->attachObject(manual);
}

PhysicDebugGraphicObject::~PhysicDebugGraphicObject() {

}

void PhysicDebugGraphicObject::Update(float DeltaTime) {

}

Error PhysicDebugGraphicObject::ChangeOccurred(ISystemObject* systemObject, System::Changes::BitMask ChangeType) {
    if (ChangeType & Schema::EntityChange::PhysicDebug) {
        auto physicDebug = systemObject->getComponent<Schema::Components::PhysicDebug>();
        for (auto it : *physicDebug->capsules()) {
            //it->origin()->x()
        }
    }
    return 0;
}







