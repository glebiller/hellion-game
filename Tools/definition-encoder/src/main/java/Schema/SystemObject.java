// automatically generated, do not modify

package Schema;

import java.nio.*;
import java.lang.*;
import java.util.*;
import com.google.flatbuffers.*;

public class SystemObject extends Table {
  public static SystemObject getRootAsSystemObject(ByteBuffer _bb) { _bb.order(ByteOrder.LITTLE_ENDIAN); return (new SystemObject()).__init(_bb.getInt(_bb.position()) + _bb.position(), _bb); }
  public SystemObject __init(int _i, ByteBuffer _bb) { bb_pos = _i; bb = _bb; return this; }

  public String type() { int o = __offset(4); return o != 0 ? __string(o + bb_pos) : null; }
  public ByteBuffer typeAsByteBuffer() { return __vector_as_bytebuffer(4, 1); }
  public short systemType() { int o = __offset(6); return o != 0 ? bb.getShort(o + bb_pos) : 0; }
  public Property properties(int j) { return properties(new Property(), j); }
  public Property properties(Property obj, int j) { int o = __offset(8); return o != 0 ? obj.__init(__indirect(__vector(o) + j * 4), bb) : null; }
  public int propertiesLength() { int o = __offset(8); return o != 0 ? __vector_len(o) : 0; }
  public ByteBuffer propertiesAsByteBuffer() { return __vector_as_bytebuffer(8, 4); }

  public static int createSystemObject(FlatBufferBuilder builder,
      int type,
      short systemType,
      int properties) {
    builder.startObject(3);
    SystemObject.addProperties(builder, properties);
    SystemObject.addType(builder, type);
    SystemObject.addSystemType(builder, systemType);
    return SystemObject.endSystemObject(builder);
  }

  public static void startSystemObject(FlatBufferBuilder builder) { builder.startObject(3); }
  public static void addType(FlatBufferBuilder builder, int typeOffset) { builder.addOffset(0, typeOffset, 0); }
  public static void addSystemType(FlatBufferBuilder builder, short systemType) { builder.addShort(1, systemType, 0); }
  public static void addProperties(FlatBufferBuilder builder, int propertiesOffset) { builder.addOffset(2, propertiesOffset, 0); }
  public static int createPropertiesVector(FlatBufferBuilder builder, int[] data) { builder.startVector(4, data.length, 4); for (int i = data.length - 1; i >= 0; i--) builder.addOffset(data[i]); return builder.endVector(); }
  public static void startPropertiesVector(FlatBufferBuilder builder, int numElems) { builder.startVector(4, numElems, 4); }
  public static int endSystemObject(FlatBufferBuilder builder) {
    int o = builder.endObject();
    builder.required(o, 4);  // type
    return o;
  }
};

