
#include "Proto/Definition/GlobalDefinition.pb.h"
#include "Proto/Definition/SceneDefinition.pb.h"

typedef google::protobuf::RepeatedPtrField<std::string>                         ProtoStringList;
typedef google::protobuf::RepeatedPtrField<PropertyProto>                       ProtoPropertyList;
typedef google::protobuf::RepeatedPtrField<SystemProto>                         ProtoSystemList;
typedef google::protobuf::RepeatedPtrField<std::string>                ProtoSceneList;
typedef google::protobuf::RepeatedPtrField<ObjectProto>                         ProtoObjectList;
typedef google::protobuf::RepeatedPtrField<SceneDefinitionProto::LinkProto>     ProtoLinksList;

typedef google::protobuf::RepeatedPtrField<ObjectProto>                         ProtoObjectPropertiesList;
typedef google::protobuf::RepeatedPtrField<ObjectProto::SystemObjectProto>      ProtoSystemPropertiesList;
