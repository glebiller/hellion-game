#include "DebugDrawer.h"

#include <Framework/include/schema/entity_change_generated.h>
#include <Scene.h>

DebugDrawer::DebugDrawer(ISystemScene &pSystemScene, UObject &entity, const Schema::SystemComponent& component) :
        ISystemObject(&pSystemScene, &entity, component),
        mDebugModes(DBG_DrawWireframe),
        connectedClient_(boost::none),
        heartbeat_delay_(500000000LL) /* 500ms */,
        running(true) {
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
    if (isRunning()) {
        auto vectorFrom = Schema::Components::PositionVector(from.x(), from.y(), from.z());
        auto vectorTo = Schema::Components::PositionVector(to.x(), to.y(), to.z());
        auto line = Schema::Components::CreateDebugLine(builder_, &vectorFrom, &vectorTo);
        lines_.push_back(line);
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
    }
    builder_.Clear();
    lines_.clear();
    planes_.clear();
    capsules_.clear();
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
    btIDebugDraw::drawCapsule(radius, halfHeight, upAxis, transform, color);
    /*if (isRunning()) {
        auto tOrigin = transform.getOrigin();
        auto origin = Schema::Components::PositionVector(tOrigin.x(), tOrigin.y(), tOrigin.z());
        auto capsule = Schema::Components::CreateDebugCapsule(builder_, radius, halfHeight, upAxis, &origin);
        capsules_.push_back(capsule);
    }*/
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
    auto lines = builder_.CreateVector(lines_);
    auto planes = builder_.CreateVector(planes_);
    auto capsules = builder_.CreateVector(capsules_);
    auto physicDebug = Schema::Components::CreatePhysicDebug(builder_, lines, planes, capsules);
    builder_.Finish(physicDebug);
    physicDebug_ = flatbuffers::GetMutableRoot<Schema::Components::PhysicDebug>(builder_.GetBufferPointer());

    if (isRunning()) {
        //server_.send(connectedClient_.get(), builder_.GetBufferPointer(), builder_.GetSize(), websocketpp::frame::opcode::binary);
        heartbeat_.start();
    }
    PostChanges(Schema::EntityChange::PhysicDebug);
}

Error DebugDrawer::ChangeOccurred(ISystemObject* systemObject, System::Changes::BitMask ChangeType) {
    if (ChangeType & Schema::EntityChange::InputTrigger) {
        running = !running;
    }
}

bool DebugDrawer::isRunning() const {
    return running /*heartbeat_.is_stopped() && connectedClient_.is_initialized()*/;
}

void DebugDrawer::Update(float DeltaTime) {
    update();
}

void DebugDrawer::drawTriangle(const btVector3& v0, const btVector3& v1, const btVector3& v2, const btVector3& color,
                               btScalar scalar) {
    // Do nothing for performance
    //btIDebugDraw::drawTriangle(v0, v1, v2, color, scalar);
}









