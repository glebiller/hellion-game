#include "Object/PhysicDebugGraphicObject.h"

#include <schema/physic_components_generated.h>
#include "GraphicScene.h"

PhysicDebugGraphicObject::PhysicDebugGraphicObject(ISystemScene& pSystemScene, UObject& entity,
                                                   const Schema::SystemComponent& component)
        : ISystemObject(&pSystemScene, &entity, component) {
    auto sceneManager = GetSystemScene<GraphicScene>()->getSceneManager();
    lines_ = sceneManager->createManualObject();
    lines_->begin("BaseWhiteNoLighting", Ogre::RenderOperation::OT_LINE_LIST);
    lines_->position(Ogre::Vector3::ZERO);
    lines_->position(Ogre::Vector3::ZERO);
    lines_->end();
    sceneManager->getRootSceneNode()->attachObject(lines_);
}

PhysicDebugGraphicObject::~PhysicDebugGraphicObject() {

}

void PhysicDebugGraphicObject::Update(float DeltaTime) {
}

Error PhysicDebugGraphicObject::ChangeOccurred(ISystemObject* systemObject, IObserver::Changes changes) {
    if (changes & Schema::EntityChange::PhysicDebug) {
        lines_->beginUpdate(0);
        lines_->colour(Ogre::ColourValue::White);
        auto physicDebug = systemObject->getComponent<Schema::Components::PhysicDebug>();
        /*for (auto it : *physicDebug->capsules()) {
            auto origin = it->origin();
            lines_->position(origin->x(), origin->y(), origin->z());
            lines_->position(origin->x(), origin->y() + it->radius() + 10, origin->z());
        }*/
        lines_->colour(Ogre::ColourValue::Blue);
        auto size = physicDebug->lines()->size();
        for (auto it : *physicDebug->lines()) {
            auto from = it->from();
            lines_->position(from->x(), from->y(), from->z());
            auto to = it->to();
            lines_->position(to->x(), to->y(), to->z());
        }
        lines_->end();
    }
    return 0;
}







