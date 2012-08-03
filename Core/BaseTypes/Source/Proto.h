
#include "Proto/gdf.pb.h"
#include "Proto/sdf.pb.h"

typedef google::protobuf::RepeatedPtrField<std::string>                 ProtoStringList;
typedef google::protobuf::RepeatedPtrField<Proto::Property>             ProtoPropertyList;
typedef google::protobuf::RepeatedPtrField<Proto::Gdf_System>           ProtoSystemList;
typedef google::protobuf::RepeatedPtrField<Proto::Gdf_Scene>            ProtoSceneList;
typedef google::protobuf::RepeatedPtrField<Proto::Sdf_Object>           ProtoObjectList;
typedef google::protobuf::RepeatedPtrField<Proto::Sdf_Link>             ProtoLinksList;

typedef google::protobuf::RepeatedPtrField<Proto::Sdf_ObjectProperties> ProtoObjectPropertiesList;
typedef google::protobuf::RepeatedPtrField<Proto::Sdf_SystemProperties> ProtoSystemPropertiesList;
