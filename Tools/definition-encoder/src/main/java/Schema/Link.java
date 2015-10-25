// automatically generated, do not modify

package Schema;

import java.nio.*;
import java.lang.*;
import java.util.*;
import com.google.flatbuffers.*;

public class Link extends Table {
  public static Link getRootAsLink(ByteBuffer _bb) { _bb.order(ByteOrder.LITTLE_ENDIAN); return (new Link()).__init(_bb.getInt(_bb.position()) + _bb.position(), _bb); }
  public Link __init(int _i, ByteBuffer _bb) { bb_pos = _i; bb = _bb; return this; }

  public String subject() { int o = __offset(4); return o != 0 ? __string(o + bb_pos) : null; }
  public ByteBuffer subjectAsByteBuffer() { return __vector_as_bytebuffer(4, 1); }
  public String observer() { int o = __offset(6); return o != 0 ? __string(o + bb_pos) : null; }
  public ByteBuffer observerAsByteBuffer() { return __vector_as_bytebuffer(6, 1); }
  public short subjectSystemType() { int o = __offset(8); return o != 0 ? bb.getShort(o + bb_pos) : 0; }
  public short observerSystemType() { int o = __offset(10); return o != 0 ? bb.getShort(o + bb_pos) : 0; }

  public static int createLink(FlatBufferBuilder builder,
      int subject,
      int observer,
      short subjectSystemType,
      short observerSystemType) {
    builder.startObject(4);
    Link.addObserver(builder, observer);
    Link.addSubject(builder, subject);
    Link.addObserverSystemType(builder, observerSystemType);
    Link.addSubjectSystemType(builder, subjectSystemType);
    return Link.endLink(builder);
  }

  public static void startLink(FlatBufferBuilder builder) { builder.startObject(4); }
  public static void addSubject(FlatBufferBuilder builder, int subjectOffset) { builder.addOffset(0, subjectOffset, 0); }
  public static void addObserver(FlatBufferBuilder builder, int observerOffset) { builder.addOffset(1, observerOffset, 0); }
  public static void addSubjectSystemType(FlatBufferBuilder builder, short subjectSystemType) { builder.addShort(2, subjectSystemType, 0); }
  public static void addObserverSystemType(FlatBufferBuilder builder, short observerSystemType) { builder.addShort(3, observerSystemType, 0); }
  public static int endLink(FlatBufferBuilder builder) {
    int o = builder.endObject();
    return o;
  }
};

