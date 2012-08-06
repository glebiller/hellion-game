// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: system.proto

#ifndef PROTOBUF_system_2eproto__INCLUDED
#define PROTOBUF_system_2eproto__INCLUDED

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
#include "property.pb.h"
// @@protoc_insertion_point(includes)

// Internal implementation detail -- do not call these.
void  protobuf_AddDesc_system_2eproto();
void protobuf_AssignDesc_system_2eproto();
void protobuf_ShutdownFile_system_2eproto();

class System;

enum System_Type {
  System_Type_AI = 0,
  System_Type_AUDIO = 1,
  System_Type_GEOMETRY = 2,
  System_Type_GRAPHIC = 3,
  System_Type_INPUT = 4,
  System_Type_PHYSIC = 5
};
bool System_Type_IsValid(int value);
const System_Type System_Type_Type_MIN = System_Type_AI;
const System_Type System_Type_Type_MAX = System_Type_PHYSIC;
const int System_Type_Type_ARRAYSIZE = System_Type_Type_MAX + 1;

const ::google::protobuf::EnumDescriptor* System_Type_descriptor();
inline const ::std::string& System_Type_Name(System_Type value) {
  return ::google::protobuf::internal::NameOfEnum(
    System_Type_descriptor(), value);
}
inline bool System_Type_Parse(
    const ::std::string& name, System_Type* value) {
  return ::google::protobuf::internal::ParseNamedEnum<System_Type>(
    System_Type_descriptor(), name, value);
}
// ===================================================================

class System : public ::google::protobuf::Message {
 public:
  System();
  virtual ~System();
  
  System(const System& from);
  
  inline System& operator=(const System& from) {
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
  static const System& default_instance();
  
  void Swap(System* other);
  
  // implements Message ----------------------------------------------
  
  System* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const System& from);
  void MergeFrom(const System& from);
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
  
  typedef System_Type Type;
  static const Type AI = System_Type_AI;
  static const Type AUDIO = System_Type_AUDIO;
  static const Type GEOMETRY = System_Type_GEOMETRY;
  static const Type GRAPHIC = System_Type_GRAPHIC;
  static const Type INPUT = System_Type_INPUT;
  static const Type PHYSIC = System_Type_PHYSIC;
  static inline bool Type_IsValid(int value) {
    return System_Type_IsValid(value);
  }
  static const Type Type_MIN =
    System_Type_Type_MIN;
  static const Type Type_MAX =
    System_Type_Type_MAX;
  static const int Type_ARRAYSIZE =
    System_Type_Type_ARRAYSIZE;
  static inline const ::google::protobuf::EnumDescriptor*
  Type_descriptor() {
    return System_Type_descriptor();
  }
  static inline const ::std::string& Type_Name(Type value) {
    return System_Type_Name(value);
  }
  static inline bool Type_Parse(const ::std::string& name,
      Type* value) {
    return System_Type_Parse(name, value);
  }
  
  // accessors -------------------------------------------------------
  
  // required .System.Type type = 1;
  inline bool has_type() const;
  inline void clear_type();
  static const int kTypeFieldNumber = 1;
  inline ::System_Type type() const;
  inline void set_type(::System_Type value);
  
  // repeated .Property properties = 2;
  inline int properties_size() const;
  inline void clear_properties();
  static const int kPropertiesFieldNumber = 2;
  inline const ::Property& properties(int index) const;
  inline ::Property* mutable_properties(int index);
  inline ::Property* add_properties();
  inline const ::google::protobuf::RepeatedPtrField< ::Property >&
      properties() const;
  inline ::google::protobuf::RepeatedPtrField< ::Property >*
      mutable_properties();
  
  // @@protoc_insertion_point(class_scope:System)
 private:
  inline void set_has_type();
  inline void clear_has_type();
  
  ::google::protobuf::UnknownFieldSet _unknown_fields_;
  
  ::google::protobuf::RepeatedPtrField< ::Property > properties_;
  int type_;
  
  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(2 + 31) / 32];
  
  friend void  protobuf_AddDesc_system_2eproto();
  friend void protobuf_AssignDesc_system_2eproto();
  friend void protobuf_ShutdownFile_system_2eproto();
  
  void InitAsDefaultInstance();
  static System* default_instance_;
};
// ===================================================================


// ===================================================================

// System

// required .System.Type type = 1;
inline bool System::has_type() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void System::set_has_type() {
  _has_bits_[0] |= 0x00000001u;
}
inline void System::clear_has_type() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void System::clear_type() {
  type_ = 0;
  clear_has_type();
}
inline ::System_Type System::type() const {
  return static_cast< ::System_Type >(type_);
}
inline void System::set_type(::System_Type value) {
  GOOGLE_DCHECK(::System_Type_IsValid(value));
  set_has_type();
  type_ = value;
}

// repeated .Property properties = 2;
inline int System::properties_size() const {
  return properties_.size();
}
inline void System::clear_properties() {
  properties_.Clear();
}
inline const ::Property& System::properties(int index) const {
  return properties_.Get(index);
}
inline ::Property* System::mutable_properties(int index) {
  return properties_.Mutable(index);
}
inline ::Property* System::add_properties() {
  return properties_.Add();
}
inline const ::google::protobuf::RepeatedPtrField< ::Property >&
System::properties() const {
  return properties_;
}
inline ::google::protobuf::RepeatedPtrField< ::Property >*
System::mutable_properties() {
  return &properties_;
}


// @@protoc_insertion_point(namespace_scope)

#ifndef SWIG
namespace google {
namespace protobuf {

template <>
inline const EnumDescriptor* GetEnumDescriptor< ::System_Type>() {
  return ::System_Type_descriptor();
}

}  // namespace google
}  // namespace protobuf
#endif  // SWIG

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_system_2eproto__INCLUDED
