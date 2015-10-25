// automatically generated, do not modify

package Schema;

import java.nio.*;
import java.lang.*;
import java.util.*;
import com.google.flatbuffers.*;

public class Property extends Table {
  public static Property getRootAsProperty(ByteBuffer _bb) { _bb.order(ByteOrder.LITTLE_ENDIAN); return (new Property()).__init(_bb.getInt(_bb.position()) + _bb.position(), _bb); }
  public Property __init(int _i, ByteBuffer _bb) { bb_pos = _i; bb = _bb; return this; }

  public String name() { int o = __offset(4); return o != 0 ? __string(o + bb_pos) : null; }
  public ByteBuffer nameAsByteBuffer() { return __vector_as_bytebuffer(4, 1); }
  public String value(int j) { int o = __offset(6); return o != 0 ? __string(__vector(o) + j * 4) : null; }
  public int valueLength() { int o = __offset(6); return o != 0 ? __vector_len(o) : 0; }
  public ByteBuffer valueAsByteBuffer() { return __vector_as_bytebuffer(6, 4); }

  public static int createProperty(FlatBufferBuilder builder,
      int name,
      int value) {
    builder.startObject(2);
    Property.addValue(builder, value);
    Property.addName(builder, name);
    return Property.endProperty(builder);
  }

  public static void startProperty(FlatBufferBuilder builder) { builder.startObject(2); }
  public static void addName(FlatBufferBuilder builder, int nameOffset) { builder.addOffset(0, nameOffset, 0); }
  public static void addValue(FlatBufferBuilder builder, int valueOffset) { builder.addOffset(1, valueOffset, 0); }
  public static int createValueVector(FlatBufferBuilder builder, int[] data) { builder.startVector(4, data.length, 4); for (int i = data.length - 1; i >= 0; i--) builder.addOffset(data[i]); return builder.endVector(); }
  public static void startValueVector(FlatBufferBuilder builder, int numElems) { builder.startVector(4, numElems, 4); }
  public static int endProperty(FlatBufferBuilder builder) {
    int o = builder.endObject();
    builder.required(o, 4);  // name
    return o;
  }
};

