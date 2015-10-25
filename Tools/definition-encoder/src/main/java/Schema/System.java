// automatically generated, do not modify

package Schema;

import java.nio.*;
import java.lang.*;
import java.util.*;
import com.google.flatbuffers.*;

public class System extends Table {
  public static System getRootAsSystem(ByteBuffer _bb) { _bb.order(ByteOrder.LITTLE_ENDIAN); return (new System()).__init(_bb.getInt(_bb.position()) + _bb.position(), _bb); }
  public System __init(int _i, ByteBuffer _bb) { bb_pos = _i; bb = _bb; return this; }

  public short type() { int o = __offset(4); return o != 0 ? bb.getShort(o + bb_pos) : 0; }
  public Property properties(int j) { return properties(new Property(), j); }
  public Property properties(Property obj, int j) { int o = __offset(6); return o != 0 ? obj.__init(__indirect(__vector(o) + j * 4), bb) : null; }
  public int propertiesLength() { int o = __offset(6); return o != 0 ? __vector_len(o) : 0; }
  public ByteBuffer propertiesAsByteBuffer() { return __vector_as_bytebuffer(6, 4); }

  public static int createSystem(FlatBufferBuilder builder,
      short type,
      int properties) {
    builder.startObject(2);
    System.addProperties(builder, properties);
    System.addType(builder, type);
    return System.endSystem(builder);
  }

  public static void startSystem(FlatBufferBuilder builder) { builder.startObject(2); }
  public static void addType(FlatBufferBuilder builder, short type) { builder.addShort(0, type, 0); }
  public static void addProperties(FlatBufferBuilder builder, int propertiesOffset) { builder.addOffset(1, propertiesOffset, 0); }
  public static int createPropertiesVector(FlatBufferBuilder builder, int[] data) { builder.startVector(4, data.length, 4); for (int i = data.length - 1; i >= 0; i--) builder.addOffset(data[i]); return builder.endVector(); }
  public static void startPropertiesVector(FlatBufferBuilder builder, int numElems) { builder.startVector(4, numElems, 4); }
  public static int endSystem(FlatBufferBuilder builder) {
    int o = builder.endObject();
    return o;
  }
};

