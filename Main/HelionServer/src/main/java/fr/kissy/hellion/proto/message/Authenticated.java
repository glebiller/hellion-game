// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: Proto/Message/Authenticated.proto

package fr.kissy.hellion.proto.message;

public final class Authenticated {
  private Authenticated() {}
  public static void registerAllExtensions(
      com.google.protobuf.ExtensionRegistry registry) {
  }
  public interface AuthenticatedProtoOrBuilder
      extends com.google.protobuf.MessageOrBuilder {
    
    // repeated .ObjectProto players = 1;
    java.util.List<fr.kissy.hellion.proto.common.ObjectDto.ObjectProto> 
        getPlayersList();
    fr.kissy.hellion.proto.common.ObjectDto.ObjectProto getPlayers(int index);
    int getPlayersCount();
    java.util.List<? extends fr.kissy.hellion.proto.common.ObjectDto.ObjectProtoOrBuilder> 
        getPlayersOrBuilderList();
    fr.kissy.hellion.proto.common.ObjectDto.ObjectProtoOrBuilder getPlayersOrBuilder(
        int index);
  }
  public static final class AuthenticatedProto extends
      com.google.protobuf.GeneratedMessage
      implements AuthenticatedProtoOrBuilder {
    // Use AuthenticatedProto.newBuilder() to construct.
    private AuthenticatedProto(Builder builder) {
      super(builder);
    }
    private AuthenticatedProto(boolean noInit) {}
    
    private static final AuthenticatedProto defaultInstance;
    public static AuthenticatedProto getDefaultInstance() {
      return defaultInstance;
    }
    
    public AuthenticatedProto getDefaultInstanceForType() {
      return defaultInstance;
    }
    
    public static final com.google.protobuf.Descriptors.Descriptor
        getDescriptor() {
      return fr.kissy.hellion.proto.message.Authenticated.internal_static_AuthenticatedProto_descriptor;
    }
    
    protected com.google.protobuf.GeneratedMessage.FieldAccessorTable
        internalGetFieldAccessorTable() {
      return fr.kissy.hellion.proto.message.Authenticated.internal_static_AuthenticatedProto_fieldAccessorTable;
    }
    
    // repeated .ObjectProto players = 1;
    public static final int PLAYERS_FIELD_NUMBER = 1;
    private java.util.List<fr.kissy.hellion.proto.common.ObjectDto.ObjectProto> players_;
    public java.util.List<fr.kissy.hellion.proto.common.ObjectDto.ObjectProto> getPlayersList() {
      return players_;
    }
    public java.util.List<? extends fr.kissy.hellion.proto.common.ObjectDto.ObjectProtoOrBuilder> 
        getPlayersOrBuilderList() {
      return players_;
    }
    public int getPlayersCount() {
      return players_.size();
    }
    public fr.kissy.hellion.proto.common.ObjectDto.ObjectProto getPlayers(int index) {
      return players_.get(index);
    }
    public fr.kissy.hellion.proto.common.ObjectDto.ObjectProtoOrBuilder getPlayersOrBuilder(
        int index) {
      return players_.get(index);
    }
    
    private void initFields() {
      players_ = java.util.Collections.emptyList();
    }
    private byte memoizedIsInitialized = -1;
    public final boolean isInitialized() {
      byte isInitialized = memoizedIsInitialized;
      if (isInitialized != -1) return isInitialized == 1;
      
      for (int i = 0; i < getPlayersCount(); i++) {
        if (!getPlayers(i).isInitialized()) {
          memoizedIsInitialized = 0;
          return false;
        }
      }
      memoizedIsInitialized = 1;
      return true;
    }
    
    public void writeTo(com.google.protobuf.CodedOutputStream output)
                        throws java.io.IOException {
      getSerializedSize();
      for (int i = 0; i < players_.size(); i++) {
        output.writeMessage(1, players_.get(i));
      }
      getUnknownFields().writeTo(output);
    }
    
    private int memoizedSerializedSize = -1;
    public int getSerializedSize() {
      int size = memoizedSerializedSize;
      if (size != -1) return size;
    
      size = 0;
      for (int i = 0; i < players_.size(); i++) {
        size += com.google.protobuf.CodedOutputStream
          .computeMessageSize(1, players_.get(i));
      }
      size += getUnknownFields().getSerializedSize();
      memoizedSerializedSize = size;
      return size;
    }
    
    private static final long serialVersionUID = 0L;
    @java.lang.Override
    protected java.lang.Object writeReplace()
        throws java.io.ObjectStreamException {
      return super.writeReplace();
    }
    
    public static fr.kissy.hellion.proto.message.Authenticated.AuthenticatedProto parseFrom(
        com.google.protobuf.ByteString data)
        throws com.google.protobuf.InvalidProtocolBufferException {
      return newBuilder().mergeFrom(data).buildParsed();
    }
    public static fr.kissy.hellion.proto.message.Authenticated.AuthenticatedProto parseFrom(
        com.google.protobuf.ByteString data,
        com.google.protobuf.ExtensionRegistryLite extensionRegistry)
        throws com.google.protobuf.InvalidProtocolBufferException {
      return newBuilder().mergeFrom(data, extensionRegistry)
               .buildParsed();
    }
    public static fr.kissy.hellion.proto.message.Authenticated.AuthenticatedProto parseFrom(byte[] data)
        throws com.google.protobuf.InvalidProtocolBufferException {
      return newBuilder().mergeFrom(data).buildParsed();
    }
    public static fr.kissy.hellion.proto.message.Authenticated.AuthenticatedProto parseFrom(
        byte[] data,
        com.google.protobuf.ExtensionRegistryLite extensionRegistry)
        throws com.google.protobuf.InvalidProtocolBufferException {
      return newBuilder().mergeFrom(data, extensionRegistry)
               .buildParsed();
    }
    public static fr.kissy.hellion.proto.message.Authenticated.AuthenticatedProto parseFrom(java.io.InputStream input)
        throws java.io.IOException {
      return newBuilder().mergeFrom(input).buildParsed();
    }
    public static fr.kissy.hellion.proto.message.Authenticated.AuthenticatedProto parseFrom(
        java.io.InputStream input,
        com.google.protobuf.ExtensionRegistryLite extensionRegistry)
        throws java.io.IOException {
      return newBuilder().mergeFrom(input, extensionRegistry)
               .buildParsed();
    }
    public static fr.kissy.hellion.proto.message.Authenticated.AuthenticatedProto parseDelimitedFrom(java.io.InputStream input)
        throws java.io.IOException {
      Builder builder = newBuilder();
      if (builder.mergeDelimitedFrom(input)) {
        return builder.buildParsed();
      } else {
        return null;
      }
    }
    public static fr.kissy.hellion.proto.message.Authenticated.AuthenticatedProto parseDelimitedFrom(
        java.io.InputStream input,
        com.google.protobuf.ExtensionRegistryLite extensionRegistry)
        throws java.io.IOException {
      Builder builder = newBuilder();
      if (builder.mergeDelimitedFrom(input, extensionRegistry)) {
        return builder.buildParsed();
      } else {
        return null;
      }
    }
    public static fr.kissy.hellion.proto.message.Authenticated.AuthenticatedProto parseFrom(
        com.google.protobuf.CodedInputStream input)
        throws java.io.IOException {
      return newBuilder().mergeFrom(input).buildParsed();
    }
    public static fr.kissy.hellion.proto.message.Authenticated.AuthenticatedProto parseFrom(
        com.google.protobuf.CodedInputStream input,
        com.google.protobuf.ExtensionRegistryLite extensionRegistry)
        throws java.io.IOException {
      return newBuilder().mergeFrom(input, extensionRegistry)
               .buildParsed();
    }
    
    public static Builder newBuilder() { return Builder.create(); }
    public Builder newBuilderForType() { return newBuilder(); }
    public static Builder newBuilder(fr.kissy.hellion.proto.message.Authenticated.AuthenticatedProto prototype) {
      return newBuilder().mergeFrom(prototype);
    }
    public Builder toBuilder() { return newBuilder(this); }
    
    @java.lang.Override
    protected Builder newBuilderForType(
        com.google.protobuf.GeneratedMessage.BuilderParent parent) {
      Builder builder = new Builder(parent);
      return builder;
    }
    public static final class Builder extends
        com.google.protobuf.GeneratedMessage.Builder<Builder>
       implements fr.kissy.hellion.proto.message.Authenticated.AuthenticatedProtoOrBuilder {
      public static final com.google.protobuf.Descriptors.Descriptor
          getDescriptor() {
        return fr.kissy.hellion.proto.message.Authenticated.internal_static_AuthenticatedProto_descriptor;
      }
      
      protected com.google.protobuf.GeneratedMessage.FieldAccessorTable
          internalGetFieldAccessorTable() {
        return fr.kissy.hellion.proto.message.Authenticated.internal_static_AuthenticatedProto_fieldAccessorTable;
      }
      
      // Construct using fr.kissy.hellion.proto.message.Authenticated.AuthenticatedProto.newBuilder()
      private Builder() {
        maybeForceBuilderInitialization();
      }
      
      private Builder(BuilderParent parent) {
        super(parent);
        maybeForceBuilderInitialization();
      }
      private void maybeForceBuilderInitialization() {
        if (com.google.protobuf.GeneratedMessage.alwaysUseFieldBuilders) {
          getPlayersFieldBuilder();
        }
      }
      private static Builder create() {
        return new Builder();
      }
      
      public Builder clear() {
        super.clear();
        if (playersBuilder_ == null) {
          players_ = java.util.Collections.emptyList();
          bitField0_ = (bitField0_ & ~0x00000001);
        } else {
          playersBuilder_.clear();
        }
        return this;
      }
      
      public Builder clone() {
        return create().mergeFrom(buildPartial());
      }
      
      public com.google.protobuf.Descriptors.Descriptor
          getDescriptorForType() {
        return fr.kissy.hellion.proto.message.Authenticated.AuthenticatedProto.getDescriptor();
      }
      
      public fr.kissy.hellion.proto.message.Authenticated.AuthenticatedProto getDefaultInstanceForType() {
        return fr.kissy.hellion.proto.message.Authenticated.AuthenticatedProto.getDefaultInstance();
      }
      
      public fr.kissy.hellion.proto.message.Authenticated.AuthenticatedProto build() {
        fr.kissy.hellion.proto.message.Authenticated.AuthenticatedProto result = buildPartial();
        if (!result.isInitialized()) {
          throw newUninitializedMessageException(result);
        }
        return result;
      }
      
      private fr.kissy.hellion.proto.message.Authenticated.AuthenticatedProto buildParsed()
          throws com.google.protobuf.InvalidProtocolBufferException {
        fr.kissy.hellion.proto.message.Authenticated.AuthenticatedProto result = buildPartial();
        if (!result.isInitialized()) {
          throw newUninitializedMessageException(
            result).asInvalidProtocolBufferException();
        }
        return result;
      }
      
      public fr.kissy.hellion.proto.message.Authenticated.AuthenticatedProto buildPartial() {
        fr.kissy.hellion.proto.message.Authenticated.AuthenticatedProto result = new fr.kissy.hellion.proto.message.Authenticated.AuthenticatedProto(this);
        int from_bitField0_ = bitField0_;
        if (playersBuilder_ == null) {
          if (((bitField0_ & 0x00000001) == 0x00000001)) {
            players_ = java.util.Collections.unmodifiableList(players_);
            bitField0_ = (bitField0_ & ~0x00000001);
          }
          result.players_ = players_;
        } else {
          result.players_ = playersBuilder_.build();
        }
        onBuilt();
        return result;
      }
      
      public Builder mergeFrom(com.google.protobuf.Message other) {
        if (other instanceof fr.kissy.hellion.proto.message.Authenticated.AuthenticatedProto) {
          return mergeFrom((fr.kissy.hellion.proto.message.Authenticated.AuthenticatedProto)other);
        } else {
          super.mergeFrom(other);
          return this;
        }
      }
      
      public Builder mergeFrom(fr.kissy.hellion.proto.message.Authenticated.AuthenticatedProto other) {
        if (other == fr.kissy.hellion.proto.message.Authenticated.AuthenticatedProto.getDefaultInstance()) return this;
        if (playersBuilder_ == null) {
          if (!other.players_.isEmpty()) {
            if (players_.isEmpty()) {
              players_ = other.players_;
              bitField0_ = (bitField0_ & ~0x00000001);
            } else {
              ensurePlayersIsMutable();
              players_.addAll(other.players_);
            }
            onChanged();
          }
        } else {
          if (!other.players_.isEmpty()) {
            if (playersBuilder_.isEmpty()) {
              playersBuilder_.dispose();
              playersBuilder_ = null;
              players_ = other.players_;
              bitField0_ = (bitField0_ & ~0x00000001);
              playersBuilder_ = 
                com.google.protobuf.GeneratedMessage.alwaysUseFieldBuilders ?
                   getPlayersFieldBuilder() : null;
            } else {
              playersBuilder_.addAllMessages(other.players_);
            }
          }
        }
        this.mergeUnknownFields(other.getUnknownFields());
        return this;
      }
      
      public final boolean isInitialized() {
        for (int i = 0; i < getPlayersCount(); i++) {
          if (!getPlayers(i).isInitialized()) {
            
            return false;
          }
        }
        return true;
      }
      
      public Builder mergeFrom(
          com.google.protobuf.CodedInputStream input,
          com.google.protobuf.ExtensionRegistryLite extensionRegistry)
          throws java.io.IOException {
        com.google.protobuf.UnknownFieldSet.Builder unknownFields =
          com.google.protobuf.UnknownFieldSet.newBuilder(
            this.getUnknownFields());
        while (true) {
          int tag = input.readTag();
          switch (tag) {
            case 0:
              this.setUnknownFields(unknownFields.build());
              onChanged();
              return this;
            default: {
              if (!parseUnknownField(input, unknownFields,
                                     extensionRegistry, tag)) {
                this.setUnknownFields(unknownFields.build());
                onChanged();
                return this;
              }
              break;
            }
            case 10: {
              fr.kissy.hellion.proto.common.ObjectDto.ObjectProto.Builder subBuilder = fr.kissy.hellion.proto.common.ObjectDto.ObjectProto.newBuilder();
              input.readMessage(subBuilder, extensionRegistry);
              addPlayers(subBuilder.buildPartial());
              break;
            }
          }
        }
      }
      
      private int bitField0_;
      
      // repeated .ObjectProto players = 1;
      private java.util.List<fr.kissy.hellion.proto.common.ObjectDto.ObjectProto> players_ =
        java.util.Collections.emptyList();
      private void ensurePlayersIsMutable() {
        if (!((bitField0_ & 0x00000001) == 0x00000001)) {
          players_ = new java.util.ArrayList<fr.kissy.hellion.proto.common.ObjectDto.ObjectProto>(players_);
          bitField0_ |= 0x00000001;
         }
      }
      
      private com.google.protobuf.RepeatedFieldBuilder<
          fr.kissy.hellion.proto.common.ObjectDto.ObjectProto, fr.kissy.hellion.proto.common.ObjectDto.ObjectProto.Builder, fr.kissy.hellion.proto.common.ObjectDto.ObjectProtoOrBuilder> playersBuilder_;
      
      public java.util.List<fr.kissy.hellion.proto.common.ObjectDto.ObjectProto> getPlayersList() {
        if (playersBuilder_ == null) {
          return java.util.Collections.unmodifiableList(players_);
        } else {
          return playersBuilder_.getMessageList();
        }
      }
      public int getPlayersCount() {
        if (playersBuilder_ == null) {
          return players_.size();
        } else {
          return playersBuilder_.getCount();
        }
      }
      public fr.kissy.hellion.proto.common.ObjectDto.ObjectProto getPlayers(int index) {
        if (playersBuilder_ == null) {
          return players_.get(index);
        } else {
          return playersBuilder_.getMessage(index);
        }
      }
      public Builder setPlayers(
          int index, fr.kissy.hellion.proto.common.ObjectDto.ObjectProto value) {
        if (playersBuilder_ == null) {
          if (value == null) {
            throw new NullPointerException();
          }
          ensurePlayersIsMutable();
          players_.set(index, value);
          onChanged();
        } else {
          playersBuilder_.setMessage(index, value);
        }
        return this;
      }
      public Builder setPlayers(
          int index, fr.kissy.hellion.proto.common.ObjectDto.ObjectProto.Builder builderForValue) {
        if (playersBuilder_ == null) {
          ensurePlayersIsMutable();
          players_.set(index, builderForValue.build());
          onChanged();
        } else {
          playersBuilder_.setMessage(index, builderForValue.build());
        }
        return this;
      }
      public Builder addPlayers(fr.kissy.hellion.proto.common.ObjectDto.ObjectProto value) {
        if (playersBuilder_ == null) {
          if (value == null) {
            throw new NullPointerException();
          }
          ensurePlayersIsMutable();
          players_.add(value);
          onChanged();
        } else {
          playersBuilder_.addMessage(value);
        }
        return this;
      }
      public Builder addPlayers(
          int index, fr.kissy.hellion.proto.common.ObjectDto.ObjectProto value) {
        if (playersBuilder_ == null) {
          if (value == null) {
            throw new NullPointerException();
          }
          ensurePlayersIsMutable();
          players_.add(index, value);
          onChanged();
        } else {
          playersBuilder_.addMessage(index, value);
        }
        return this;
      }
      public Builder addPlayers(
          fr.kissy.hellion.proto.common.ObjectDto.ObjectProto.Builder builderForValue) {
        if (playersBuilder_ == null) {
          ensurePlayersIsMutable();
          players_.add(builderForValue.build());
          onChanged();
        } else {
          playersBuilder_.addMessage(builderForValue.build());
        }
        return this;
      }
      public Builder addPlayers(
          int index, fr.kissy.hellion.proto.common.ObjectDto.ObjectProto.Builder builderForValue) {
        if (playersBuilder_ == null) {
          ensurePlayersIsMutable();
          players_.add(index, builderForValue.build());
          onChanged();
        } else {
          playersBuilder_.addMessage(index, builderForValue.build());
        }
        return this;
      }
      public Builder addAllPlayers(
          java.lang.Iterable<? extends fr.kissy.hellion.proto.common.ObjectDto.ObjectProto> values) {
        if (playersBuilder_ == null) {
          ensurePlayersIsMutable();
          super.addAll(values, players_);
          onChanged();
        } else {
          playersBuilder_.addAllMessages(values);
        }
        return this;
      }
      public Builder clearPlayers() {
        if (playersBuilder_ == null) {
          players_ = java.util.Collections.emptyList();
          bitField0_ = (bitField0_ & ~0x00000001);
          onChanged();
        } else {
          playersBuilder_.clear();
        }
        return this;
      }
      public Builder removePlayers(int index) {
        if (playersBuilder_ == null) {
          ensurePlayersIsMutable();
          players_.remove(index);
          onChanged();
        } else {
          playersBuilder_.remove(index);
        }
        return this;
      }
      public fr.kissy.hellion.proto.common.ObjectDto.ObjectProto.Builder getPlayersBuilder(
          int index) {
        return getPlayersFieldBuilder().getBuilder(index);
      }
      public fr.kissy.hellion.proto.common.ObjectDto.ObjectProtoOrBuilder getPlayersOrBuilder(
          int index) {
        if (playersBuilder_ == null) {
          return players_.get(index);  } else {
          return playersBuilder_.getMessageOrBuilder(index);
        }
      }
      public java.util.List<? extends fr.kissy.hellion.proto.common.ObjectDto.ObjectProtoOrBuilder> 
           getPlayersOrBuilderList() {
        if (playersBuilder_ != null) {
          return playersBuilder_.getMessageOrBuilderList();
        } else {
          return java.util.Collections.unmodifiableList(players_);
        }
      }
      public fr.kissy.hellion.proto.common.ObjectDto.ObjectProto.Builder addPlayersBuilder() {
        return getPlayersFieldBuilder().addBuilder(
            fr.kissy.hellion.proto.common.ObjectDto.ObjectProto.getDefaultInstance());
      }
      public fr.kissy.hellion.proto.common.ObjectDto.ObjectProto.Builder addPlayersBuilder(
          int index) {
        return getPlayersFieldBuilder().addBuilder(
            index, fr.kissy.hellion.proto.common.ObjectDto.ObjectProto.getDefaultInstance());
      }
      public java.util.List<fr.kissy.hellion.proto.common.ObjectDto.ObjectProto.Builder> 
           getPlayersBuilderList() {
        return getPlayersFieldBuilder().getBuilderList();
      }
      private com.google.protobuf.RepeatedFieldBuilder<
          fr.kissy.hellion.proto.common.ObjectDto.ObjectProto, fr.kissy.hellion.proto.common.ObjectDto.ObjectProto.Builder, fr.kissy.hellion.proto.common.ObjectDto.ObjectProtoOrBuilder> 
          getPlayersFieldBuilder() {
        if (playersBuilder_ == null) {
          playersBuilder_ = new com.google.protobuf.RepeatedFieldBuilder<
              fr.kissy.hellion.proto.common.ObjectDto.ObjectProto, fr.kissy.hellion.proto.common.ObjectDto.ObjectProto.Builder, fr.kissy.hellion.proto.common.ObjectDto.ObjectProtoOrBuilder>(
                  players_,
                  ((bitField0_ & 0x00000001) == 0x00000001),
                  getParentForChildren(),
                  isClean());
          players_ = null;
        }
        return playersBuilder_;
      }
      
      // @@protoc_insertion_point(builder_scope:AuthenticatedProto)
    }
    
    static {
      defaultInstance = new AuthenticatedProto(true);
      defaultInstance.initFields();
    }
    
    // @@protoc_insertion_point(class_scope:AuthenticatedProto)
  }
  
  private static com.google.protobuf.Descriptors.Descriptor
    internal_static_AuthenticatedProto_descriptor;
  private static
    com.google.protobuf.GeneratedMessage.FieldAccessorTable
      internal_static_AuthenticatedProto_fieldAccessorTable;
  
  public static com.google.protobuf.Descriptors.FileDescriptor
      getDescriptor() {
    return descriptor;
  }
  private static com.google.protobuf.Descriptors.FileDescriptor
      descriptor;
  static {
    java.lang.String[] descriptorData = {
      "\n!Proto/Message/Authenticated.proto\032\031Pro" +
      "to/Common/Object.proto\032\033Proto/Common/Pro" +
      "perty.proto\"3\n\022AuthenticatedProto\022\035\n\007pla" +
      "yers\030\001 \003(\0132\014.ObjectProtoB2\n\036fr.kissy.hel" +
      "lion.proto.messageB\rAuthenticated\210\001\000"
    };
    com.google.protobuf.Descriptors.FileDescriptor.InternalDescriptorAssigner assigner =
      new com.google.protobuf.Descriptors.FileDescriptor.InternalDescriptorAssigner() {
        public com.google.protobuf.ExtensionRegistry assignDescriptors(
            com.google.protobuf.Descriptors.FileDescriptor root) {
          descriptor = root;
          internal_static_AuthenticatedProto_descriptor =
            getDescriptor().getMessageTypes().get(0);
          internal_static_AuthenticatedProto_fieldAccessorTable = new
            com.google.protobuf.GeneratedMessage.FieldAccessorTable(
              internal_static_AuthenticatedProto_descriptor,
              new java.lang.String[] { "Players", },
              fr.kissy.hellion.proto.message.Authenticated.AuthenticatedProto.class,
              fr.kissy.hellion.proto.message.Authenticated.AuthenticatedProto.Builder.class);
          return null;
        }
      };
    com.google.protobuf.Descriptors.FileDescriptor
      .internalBuildGeneratedFileFrom(descriptorData,
        new com.google.protobuf.Descriptors.FileDescriptor[] {
          fr.kissy.hellion.proto.common.ObjectDto.getDescriptor(),
          fr.kissy.hellion.proto.common.PropertyDto.getDescriptor(),
        }, assigner);
  }
  
  // @@protoc_insertion_point(outer_class_scope)
}
