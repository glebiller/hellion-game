#pragma once

#include <boost/optional.hpp>
#include <bullet/LinearMath/btIDebugDraw.h>
#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>
#include <schema/entity_change_generated.h>
#include <schema/physic_components_generated.h>
#include <boost/timer/timer.hpp>
#include <System/ISystemScene.h>
#include <Universal/UObject.h>

class DebugDrawer : public btIDebugDraw, public ISystemObject {
public:
    DebugDrawer(ISystemScene &pSystemScene, UObject &entity, const Schema::SystemComponent& component);

    ~DebugDrawer();

    virtual void drawLine(const btVector3& from, const btVector3& to, const btVector3& color) override;

    virtual void drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance,
                                  int lifeTime, const btVector3& color) override;

    virtual void reportErrorWarning(const char* warningString) override;

    virtual void draw3dText(const btVector3& location, const char* textString) override;

    virtual void drawSphere(btScalar radius, const btTransform& transform, const btVector3& color) override {
        btIDebugDraw::drawSphere(radius, transform, color);
    }

    virtual void drawSphere(const btVector3& p, btScalar radius, const btVector3& color) override {
        btIDebugDraw::drawSphere(p, radius, color);
    }

    virtual void drawTriangle(const btVector3& v0, const btVector3& v1, const btVector3& v2, const btVector3& vector3,
                              const btVector3& vector31, const btVector3& vector32, const btVector3& color,
                              btScalar alpha) override {
        btIDebugDraw::drawTriangle(v0, v1, v2, vector3, vector31, vector32, color, alpha);
    }

    virtual void drawTriangle(const btVector3& v0, const btVector3& v1, const btVector3& v2, const btVector3& color,
                              btScalar scalar) override {
        btIDebugDraw::drawTriangle(v0, v1, v2, color, scalar);
    }

    virtual void drawAabb(const btVector3& from, const btVector3& to, const btVector3& color) override {
        btIDebugDraw::drawAabb(from, to, color);
    }

    virtual void drawTransform(const btTransform& transform, btScalar orthoLen) override {
        btIDebugDraw::drawTransform(transform, orthoLen);
    }

    virtual void drawArc(const btVector3& center, const btVector3& normal, const btVector3& axis, btScalar radiusA,
                         btScalar radiusB, btScalar minAngle, btScalar maxAngle, const btVector3& color, bool drawSect,
                         btScalar stepDegrees) override {
        btIDebugDraw::drawArc(center, normal, axis, radiusA, radiusB, minAngle, maxAngle, color, drawSect, stepDegrees);
    }

    virtual void drawSpherePatch(const btVector3& center, const btVector3& up, const btVector3& axis, btScalar radius,
                                 btScalar minTh, btScalar maxTh, btScalar minPs, btScalar maxPs, const btVector3& color,
                                 btScalar stepDegrees, bool drawCenter) override {
        btIDebugDraw::drawSpherePatch(center, up, axis, radius, minTh, maxTh, minPs, maxPs, color, stepDegrees,
                                      drawCenter);
    }

    virtual void drawBox(const btVector3& bbMin, const btVector3& bbMax, const btVector3& color) override {
        btIDebugDraw::drawBox(bbMin, bbMax, color);
    }

    virtual void drawBox(const btVector3& bbMin, const btVector3& bbMax, const btTransform& trans,
                         const btVector3& color) override {
        btIDebugDraw::drawBox(bbMin, bbMax, trans, color);
    }

    virtual void drawCapsule(btScalar radius, btScalar halfHeight, int upAxis, const btTransform& transform,
                             const btVector3& color) override;

    virtual void drawCylinder(btScalar radius, btScalar halfHeight, int upAxis, const btTransform& transform,
                              const btVector3& color) override {
        btIDebugDraw::drawCylinder(radius, halfHeight, upAxis, transform, color);
    }

    virtual void drawCone(btScalar radius, btScalar height, int upAxis, const btTransform& transform,
                          const btVector3& color) override {
        btIDebugDraw::drawCone(radius, height, upAxis, transform, color);
    }

    virtual void drawPlane(const btVector3& planeNormal, btScalar planeConst, const btTransform& transform,
                           const btVector3& color) override;

    virtual void flushLines() override;


    virtual void setDebugMode(int debugMode) override;

    virtual int getDebugMode() const override;

    Error ChangeOccurred(ISystemObject* systemObject, System::Changes::BitMask ChangeType) override;

    void update();

    void Update(float DeltaTime) override;

    System::Changes::BitMask GetPotentialSystemChanges() override {
        return Schema::EntityChange::PhysicDebug;
    };

    System::Types::BitMask GetDesiredSystemChanges() override {
        return System::Changes::None;
    };


private:

    void onOpen(websocketpp::connection_hdl hdl);

    void onClose(websocketpp::connection_hdl hdl);

private:
    DebugDrawModes mDebugModes;
    websocketpp::server<websocketpp::config::asio> server_;
    boost::optional<websocketpp::connection_hdl>  connectedClient_;
    Schema::Components::PhysicDebug* physicDebug_;
    std::vector<flatbuffers::Offset<Schema::Components::DebugLine>> lines_;
    std::vector<flatbuffers::Offset<Schema::Components::DebugPlane>> planes_;
    std::vector<flatbuffers::Offset<Schema::Components::DebugCapsule>> capsules_;
    flatbuffers::FlatBufferBuilder builder_;
    boost::timer::cpu_timer  heartbeat_;
    boost::timer::nanosecond_type   heartbeat_delay_;

    bool isRunning() const;
};
