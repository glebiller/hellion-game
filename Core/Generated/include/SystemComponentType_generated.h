// automatically generated by the FlatBuffers compiler, do not modify

#ifndef FLATBUFFERS_GENERATED_SYSTEMCOMPONENTTYPE_SCHEMA_H_
#define FLATBUFFERS_GENERATED_SYSTEMCOMPONENTTYPE_SCHEMA_H_

#include "flatbuffers/flatbuffers.h"

#include "GraphicSystemComponents_generated.h"
#include "InputSystemComponents_generated.h"
#include "PhysicSystemComponents_generated.h"

namespace Schema {
struct GraphicCamera;
struct GraphicMesh;
}  // namespace Schema
namespace Schema {
struct VelocityVector;
struct InputVelocity;
}  // namespace Schema
namespace Schema {
struct PhysicPosition;
}  // namespace Schema

namespace Schema {


enum class SystemComponentType {
  NONE = 0,
  GraphicCamera = 1,
  GraphicMesh = 2,
  InputVelocity = 3,
  PhysicPosition = 4
};

inline const char **EnumNamesSystemComponentType() {
  static const char *names[] = { "NONE", "GraphicCamera", "GraphicMesh", "InputVelocity", "PhysicPosition", nullptr };
  return names;
}

inline const char *EnumNameSystemComponentType(SystemComponentType e) { return EnumNamesSystemComponentType()[static_cast<int>(e)]; }

inline bool VerifySystemComponentType(flatbuffers::Verifier &verifier, const void *union_obj, SystemComponentType type);

inline bool VerifySystemComponentType(flatbuffers::Verifier &verifier, const void *union_obj, SystemComponentType type) {
  switch (type) {
    case SystemComponentType::NONE: return true;
    case SystemComponentType::GraphicCamera: return verifier.VerifyTable(reinterpret_cast<const Schema::GraphicCamera *>(union_obj));
    case SystemComponentType::GraphicMesh: return verifier.VerifyTable(reinterpret_cast<const Schema::GraphicMesh *>(union_obj));
    case SystemComponentType::InputVelocity: return verifier.VerifyTable(reinterpret_cast<const Schema::InputVelocity *>(union_obj));
    case SystemComponentType::PhysicPosition: return verifier.VerifyTable(reinterpret_cast<const Schema::PhysicPosition *>(union_obj));
    default: return false;
  }
}

}  // namespace Schema

#endif  // FLATBUFFERS_GENERATED_SYSTEMCOMPONENTTYPE_SCHEMA_H_
