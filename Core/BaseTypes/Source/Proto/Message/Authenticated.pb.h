// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: Proto/Message/Authenticated.proto

#ifndef PROTOBUF_Proto_2fMessage_2fAuthenticated_2eproto__INCLUDED
#define PROTOBUF_Proto_2fMessage_2fAuthenticated_2eproto__INCLUDED

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
#include "Proto/Common/Object.pb.h"
#include "Proto/Common/Property.pb.h"
// @@protoc_insertion_point(includes)

// Internal implementation detail -- do not call these.
void  protobuf_AddDesc_Proto_2fMessage_2fAuthenticated_2eproto();
void protobuf_AssignDesc_Proto_2fMessage_2fAuthenticated_2eproto();
void protobuf_ShutdownFile_Proto_2fMessage_2fAuthenticated_2eproto();

class AuthenticatedProto;

// ===================================================================

class AuthenticatedProto : public ::google::protobuf::Message {
 public:
  AuthenticatedProto();
  virtual ~AuthenticatedProto();
  
  AuthenticatedProto(const AuthenticatedProto& from);
  
  inline AuthenticatedProto& operator=(const AuthenticatedProto& from) {
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
  static const AuthenticatedProto& default_instance();
  
  void Swap(AuthenticatedProto* other);
  
  // implements Message ----------------------------------------------
  
  AuthenticatedProto* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const AuthenticatedProto& from);
  void MergeFrom(const AuthenticatedProto& from);
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
  
  // accessors -------------------------------------------------------
  
  // repeated .ObjectProto players = 1;
  inline int players_size() const;
  inline void clear_players();
  static const int kPlayersFieldNumber = 1;
  inline const ::ObjectProto& players(int index) const;
  inline ::ObjectProto* mutable_players(int index);
  inline ::ObjectProto* add_players();
  inline const ::google::protobuf::RepeatedPtrField< ::ObjectProto >&
      players() const;
  inline ::google::protobuf::RepeatedPtrField< ::ObjectProto >*
      mutable_players();
  
  // @@protoc_insertion_point(class_scope:AuthenticatedProto)
 private:
  
  ::google::protobuf::UnknownFieldSet _unknown_fields_;
  
  ::google::protobuf::RepeatedPtrField< ::ObjectProto > players_;
  
  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(1 + 31) / 32];
  
  friend void  protobuf_AddDesc_Proto_2fMessage_2fAuthenticated_2eproto();
  friend void protobuf_AssignDesc_Proto_2fMessage_2fAuthenticated_2eproto();
  friend void protobuf_ShutdownFile_Proto_2fMessage_2fAuthenticated_2eproto();
  
  void InitAsDefaultInstance();
  static AuthenticatedProto* default_instance_;
};
// ===================================================================


// ===================================================================

// AuthenticatedProto

// repeated .ObjectProto players = 1;
inline int AuthenticatedProto::players_size() const {
  return players_.size();
}
inline void AuthenticatedProto::clear_players() {
  players_.Clear();
}
inline const ::ObjectProto& AuthenticatedProto::players(int index) const {
  return players_.Get(index);
}
inline ::ObjectProto* AuthenticatedProto::mutable_players(int index) {
  return players_.Mutable(index);
}
inline ::ObjectProto* AuthenticatedProto::add_players() {
  return players_.Add();
}
inline const ::google::protobuf::RepeatedPtrField< ::ObjectProto >&
AuthenticatedProto::players() const {
  return players_;
}
inline ::google::protobuf::RepeatedPtrField< ::ObjectProto >*
AuthenticatedProto::mutable_players() {
  return &players_;
}


// @@protoc_insertion_point(namespace_scope)

#ifndef SWIG
namespace google {
namespace protobuf {


}  // namespace google
}  // namespace protobuf
#endif  // SWIG

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_Proto_2fMessage_2fAuthenticated_2eproto__INCLUDED
