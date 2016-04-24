#include "component/InputTriggerComponent.h"

#include <Scene.h>

InputTriggerComponent::InputTriggerComponent(ISystemScene& pSystemScene, UObject& entity,
                                               const Schema::SystemComponent& component)
        : ISystemObject(&pSystemScene, &entity, component) {
    trigger_ = getMutableComponent<Schema::Components::InputTrigger>();
    InputScene* inputScene = GetSystemScene<InputScene>();
    triggerAction_ = inputScene->getDefaultSchema()->createAction<OISB::TriggerAction>(entity.getId() + "_Trigger");
    triggerAction_->bind("Keyboard/g");
}

InputTriggerComponent::~InputTriggerComponent() {

}

void InputTriggerComponent::Update(float DeltaTime) {
    if (triggerAction_->hasChanged() && triggerAction_->isActive()) {
        PostChanges(Schema::EntityChange::InputTrigger);
    }
}

Error InputTriggerComponent::ChangeOccurred(ISystemObject* systemObject, System::Changes::BitMask ChangeType) {
    return 0;
}



