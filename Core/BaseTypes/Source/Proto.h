
#include "Proto/Definition.pb.h"

namespace Proto {
    typedef google::protobuf::RepeatedPtrField<std::string>                 RepeatedString;
    typedef google::protobuf::RepeatedPtrField<Property>                    RepeatedProperty;
    typedef google::protobuf::RepeatedPtrField<System>                      RepeatedSystem;
    typedef google::protobuf::RepeatedPtrField<Object>                      RepeatedObject;
    typedef google::protobuf::RepeatedPtrField<SystemObject>                RepeatedSystemObject;
    typedef google::protobuf::RepeatedPtrField<Link>                        RepeatedLink;
}