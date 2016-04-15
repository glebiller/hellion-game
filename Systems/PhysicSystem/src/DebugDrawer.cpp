#include <Framework/include/schema/entity_change_generated.h>
#include <Scene.h>
#include "DebugDrawer.h"

DebugDrawer::DebugDrawer(ISystemScene &pSystemScene, UObject &entity, const Schema::SystemComponent& component) :
        ISystemObject(&pSystemScene, &entity, component),
        mDebugModes(DBG_DrawWireframe),
        connectedClient_(boost::none),
        heartbeat_delay_(500000000LL) /* 500ms */ {
    physicDebug_ = const_cast<Schema::Components::PhysicDebug*>(static_cast<const Schema::Components::PhysicDebug*>(component.data()));

    GetSystemScene<PhysicScene>()->getDynamicsWorld_()->setDebugDrawer(this);

    server_.init_asio();
    server_.listen(9002);
    server_.start_accept();
    server_.set_open_handler(websocketpp::lib::bind(&DebugDrawer::onOpen, this, websocketpp::lib::placeholders::_1));
    server_.set_close_handler(websocketpp::lib::bind(&DebugDrawer::onClose, this, websocketpp::lib::placeholders::_1));
    server_.clear_access_channels(websocketpp::log::alevel::all);
    heartbeat_.stop();
}

DebugDrawer::~DebugDrawer() {
    server_.stop_listening();
    if (connectedClient_.is_initialized()) {
        server_.close(connectedClient_.get(), websocketpp::close::status::normal, "Closing");
    }
}

void DebugDrawer::drawLine(const btVector3& from, const btVector3& to, const btVector3& color) {
    if (connectedClient_.is_initialized()) {

    }
}

void DebugDrawer::drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance,
                                   int lifeTime, const btVector3& color) {

}

void DebugDrawer::reportErrorWarning(const char* warningString) {

}

void DebugDrawer::draw3dText(const btVector3& location, const char* textString) {

}

void DebugDrawer::setDebugMode(int debugMode) {
    mDebugModes = (DebugDrawModes) debugMode;
}

int DebugDrawer::getDebugMode() const {
    return mDebugModes;
}

void DebugDrawer::update() {
    if (heartbeat_.elapsed().wall >= heartbeat_delay_) {
        heartbeat_.stop();
        builder_.Clear();
        lines_.clear();
        planes_.clear();
        capsules_.clear();
    }
    server_.poll();

}

void DebugDrawer::onOpen(websocketpp::connection_hdl hdl) {
    if (!connectedClient_.is_initialized()) {
        connectedClient_ = hdl;
        heartbeat_.start();
    }
}

void DebugDrawer::onClose(websocketpp::connection_hdl hdl) {
    connectedClient_ = boost::none;
    heartbeat_.stop();
}

void DebugDrawer::drawCapsule(btScalar radius, btScalar halfHeight, int upAxis, const btTransform& transform,
                              const btVector3& color) {
    if (isRunning()) {
        auto tOrigin = transform.getOrigin();
        auto origin = Schema::Components::PositionVector(tOrigin.x(), tOrigin.y(), tOrigin.z());
        auto capsule = Schema::Components::CreateDebugCapsule(builder_, radius, halfHeight, upAxis, &origin);
        capsules_.push_back(capsule);
    }
}

void DebugDrawer::drawPlane(const btVector3& planeNormal, btScalar planeConst, const btTransform& transform,
                            const btVector3& color) {
    if (isRunning()) {
        /*auto normal = Schema::Components::PositionVector(planeNormal.x(), planeNormal.y(), planeNormal.z());
        auto tOrigin = transform.getOrigin();
        auto origin = Schema::Components::PositionVector(tOrigin.x(), tOrigin.y(), tOrigin.z());
        auto plane = Schema::Components::CreateDebugPlane(builder_, &normal, planeConst, &origin);
        planes_.push_back(plane);*/
    }
}

void DebugDrawer::flushLines() {
    //auto lines = builder_.CreateVector(lines_);
    //auto planes = builder_.CreateVector(planes_);
    //auto capsules = builder_.CreateVector(capsules_);
    //auto debugDrawer = Schema::Components::CreateDebugDrawer(builder_, lines, planes, capsules);
    //builder_.Finish(debugDrawer);
    for (int i = 0; i < capsules_.size(); i++) {
        auto capsule = flatbuffers::Offset<Schema::Components::DebugCapsule>(capsules_[i]);
        physicDebug_->mutable_capsules()->Mutate(i, capsule);
    }

    if (isRunning()) {
        server_.send(connectedClient_.get(), builder_.GetBufferPointer(), builder_.GetSize(), websocketpp::frame::opcode::binary);
        heartbeat_.start();
    }
    PostChanges(Schema::EntityChange::PhysicDebug);
}

Error DebugDrawer::ChangeOccurred(ISystemObject* systemObject, System::Changes::BitMask ChangeType) {
    return 0;
}

bool DebugDrawer::isRunning() const {
    return heartbeat_.is_stopped() && connectedClient_.is_initialized();
}

void DebugDrawer::Update(float DeltaTime) {
    update();
}









