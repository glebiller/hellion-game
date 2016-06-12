#pragma once

#include "System/ISystemObject.h"
#include "../../../../../../Library/Caches/CLion2016.1/cmake/generated/hellion-game-b2780fa8/b2780fa8/Debug/Framework/include/schema/scene_generated.h"
#include "../../../../../../Library/Caches/CLion2016.1/cmake/generated/hellion-game-b2780fa8/b2780fa8/Debug/Framework/include/schema/entity_change_generated.h"
#include <schema/scene_generated.h>
#include <schema/entity_change_generated.h>
#include <OISB.h>

class InputTriggerComponent : public ISystemObject {
public:

    /**
     * @inheritDoc
     */
    InputTriggerComponent(ISystemScene& pSystemScene, UObject& entity, const Schema::SystemComponent& component);

    /**
     * @inheritDoc
     */
    virtual ~InputTriggerComponent();

    System::Changes::BitMask GetPotentialSystemChanges() override {
        return Schema::EntityChange::InputTrigger;
    };

    IObserver::Changes GetDesiredSystemChanges() override {
        return 0;
    };

    void Update(float DeltaTime) override;

    Error ChangeOccurred(ISystemObject* systemObject, IObserver::Changes changes) override;

private:
    Schema::Components::InputTrigger* trigger_;
    OISB::TriggerAction*    triggerAction_;
};
