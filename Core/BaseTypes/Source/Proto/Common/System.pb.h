// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: Common/System.proto

#ifndef PROTOBUF_Common_2fSystem_2eproto__INCLUDED
#define PROTOBUF_Common_2fSystem_2eproto__INCLUDED

#include <string>

#include <google/protobuf/stubs/common.h>

#if GOOGLE_PROTOBUF_VERSION < 2004000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please update
#error your headers.
#endif
#if 2004001 < GOOGLE_PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/repeated_field.h>
#include <google/protobuf/extension_set.h>
#include <google/protobuf/generated_message_reflection.h>
#include "Proto/Common/Property.pb.h"
// @@protoc_insertion_point(includes)

// Internal implementation detail -- do not call these.
void  protobuf_AddDesc_Common_2fSystem_2eproto();
void protobuf_AssignDesc_Common_2fSystem_2eproto();
void protobuf_ShutdownFile_Common_2fSystem_2eproto();

class SystemProto;

enum SystemProto_Type {
  SystemProto_Type_AI = 0,
  SystemProto_Type_AUDIO = 1,
  SystemProto_Type_GEOMETRY = 2,
  SystemProto_Type_GRAPHIC = 3,
  SystemProto_Type_INPUT = 4,
  SystemProto_Type_PHYSIC = 5
};
bool SystemProto_Type_IsValid(int value);
const SystemProto_Type SystemProto_Type_Type_MIN = SystemProto_Type_AI;
const SystemProto_Type SystemProto_Type_Type_MAX = SystemProto_Type_PHYSIC;
const int SystemProto_Type_Type_ARRAYSIZE = SystemProto_Type_Type_MAX + 1;

const ::google::protobuf::EnumDescriptor* SystemProto_Type_descriptor();
inline const ::std::string& SystemProto_Type_Name(SystemProto_Type value) {
  return ::google::protobuf::internal::NameOfEnum(
    SystemProto_Type_descriptor(), value);
}
inline bool SystemProto_Type_Parse(
    const ::std::string& name, SystemProto_Type* value) {
  return ::google::protobuf::internal::ParseNamedEnum<SystemProto_Type>(
    SystemProto_Type_descriptor(), name, value);
}
// ===================================================================

class SystemProto : public ::google::protobuf::Message {
 public:
  SystemProto();
  virtual ~SystemProto();
  
  SystemProto(const SystemProto& from);
  
  inline SystemProto& operator=(const SystemProto& from) {
    CopyFrom(from);
    return *this;
  }
  
  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _unknown_fields_;
  }
  
  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return &_unknown_fields_;
  }
  
  static const ::google::protobuf::Descriptor* descriptor();
  static const SystemProto& default_instance();
  
  void Swap(SystemProto* other);
  
  // implements Message ----------------------------------------------
  
  SystemProto* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const SystemProto& from);
  void MergeFrom(const SystemProto& from);
  void Clear();
  bool IsInitialized() const;
  
  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  public:
  
  ::google::protobuf::Metadata GetMetadata() const;
  
  // nested types ----------------------------------------------------
  
  typedef SystemProto_Type Type;
  static const Type AI = SystemProto_Type_AI;
  static const Type AUDIO = SystemProto_Type_AUDIO;
  static const Type GEOMETRY = SystemProto_Type_GEOMETRY;
  static const Type GRAPHIC = SystemProto_Type_GRAPHIC;
  static const Type INPUT = SystemProto_Type_INPUT;
  static const Type PHYSIC = SystemProto_Type_PHYSIC;
  static inline bool Type_IsValid(int value) {
    return SystemProto_Type_IsValid(value);
  }
  static const Type Type_MIN =
    SystemProto_Type_Type_MIN;
  static const Type Type_MAX =
    SystemProto_Type_Type_MAX;
  static const int Type_ARRAYSIZE =
    SystemProto_Type_Type_ARRAYSIZE;
  static inline const ::google::protobuf::EnumDescriptor*
  Type_descriptor() {
    return SystemProto_Type_descriptor();
  }
  static inline const ::std::string& Type_Name(Type value) {
    return SystemProto_Type_Name(value);
  }
  static inline bool Type_Parse(const ::std::string& name,
      Type* value) {
    return SystemProto_Type_Parse(name, value);
  }
  
  // accessors -------------------------------------------------------
  
  // required .SystemProto.Type type = 1;
  inline bool has_type() const;
  inline void clear_type();
  static const int kTypeFieldNumber = 1;
  inline ::SystemProto_Type type() const;
  inline void set_type(::SystemProto_Type value);
  
  // repeated .PropertyProto properties = 2;
  inline int properties_size() const;
  inline void clear_properties();
  static const int kPropertiesFieldNumber = 2;
  inline const ::PropertyProto& properties(int index) const;
  inline ::PropertyProto* mutable_properties(int index);
  inline ::PropertyProto* add_properties();
  inline const ::google::protobuf::RepeatedPtrField< ::PropertyProto >&
      properties() const;
  inline ::google::protobuf::RepeatedPtrField< ::PropertyProto >*
      mutable_properties();
  
  // @@protoc_insertion_point(class_scope:SystemProto)
 private:
  inline void set_has_type();
  inline void clear_has_type();
  
  ::google::protobuf::UnknownFieldSet _unknown_fields_;
  
  ::google::protobuf::RepeatedPtrField< ::PropertyProto > properties_;
  int type_;
  
  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(2 + 31) / 32];
  
  friend void  protobuf_AddDesc_Common_2fSystem_2eproto();
  friend void protobuf_AssignDesc_Common_2fSystem_2eproto();
  friend void protobuf_ShutdownFile_Common_2fSystem_2eproto();
  
  void InitAsDefaultInstance();
  static SystemProto* default_instance_;
};
// ===================================================================


// ===================================================================

// SystemProto

// required .SystemProto.Type type = 1;
inline bool SystemProto::has_type() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void SystemProto::set_has_type() {
  _has_bits_[0] |= 0x00000001u;
}
inline void SystemProto::clear_has_type() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void SystemProto::clear_type() {
  type_ = 0;
  clear_has_type();
}
inline ::SystemProto_Type SystemProto::type() const {
  return static_cast< ::SystemProto_Type >(type_);
}
inline void SystemProto::set_type(::SystemProto_Type value) {
  GOOGLE_DCHECK(::SystemProto_Type_IsValid(value));
  set_has_type();
  type_ = value;
}

// repeated .PropertyProto properties = 2;
inline int SystemProto::properties_size() const {
  return properties_.size();
}
inline void SystemProto::clear_properties() {
  properties_.Clear();
}
inline const ::PropertyProto& SystemProto::properties(int index) const {
  return properties_.Get(index);
}
inline ::PropertyProto* SystemProto::mutable_properties(int index) {
  return properties_.Mutable(index);
}
inline ::PropertyProto* SystemProto::add_properties() {
  return properties_.Add();
}
inline const ::google::protobuf::RepeatedPtrField< ::PropertyProto >&
SystemProto::properties() const {
  return properties_;
}
inline ::google::protobuf::RepeatedPtrField< ::PropertyProto >*
SystemProto::mutable_properties() {
  return &properties_;
}


// @@protoc_insertion_point(namespace_scope)

#ifndef SWIG
namespace google {
namespace protobuf {

template <>
inline const EnumDescriptor* GetEnumDescriptor< ::SystemProto_Type>() {
  return ::SystemProto_Type_descriptor();
}

}  // namespace google
}  // namespace protobuf
#endif  // SWIG

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_Common_2fSystem_2eproto__INCLUDED
