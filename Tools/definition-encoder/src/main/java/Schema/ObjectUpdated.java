// automatically generated, do not modify

package Schema;

import java.nio.*;
import java.lang.*;
import java.util.*;
import com.google.flatbuffers.*;

public class ObjectUpdated extends Table {
  public static ObjectUpdated getRootAsObjectUpdated(ByteBuffer _bb) { _bb.order(ByteOrder.LITTLE_ENDIAN); return (new ObjectUpdated()).__init(_bb.getInt(_bb.position()) + _bb.position(), _bb); }
  public ObjectUpdated __init(int _i, ByteBuffer _bb) { bb_pos = _i; bb = _bb; return this; }

  public Object objects(int j) { return objects(new Object(), j); }
  public Object objects(Object obj, int j) { int o = __offset(4); return o != 0 ? obj.__init(__indirect(__vector(o) + j * 4), bb) : null; }
  public int objectsLength() { int o = __offset(4); return o != 0 ? __vector_len(o) : 0; }
  public ByteBuffer objectsAsByteBuffer() { return __vector_as_bytebuffer(4, 4); }

  public static int createObjectUpdated(FlatBufferBuilder builder,
      int objects) {
    builder.startObject(1);
    ObjectUpdated.addObjects(builder, objects);
    return ObjectUpdated.endObjectUpdated(builder);
  }

  public static void startObjectUpdated(FlatBufferBuilder builder) { builder.startObject(1); }
  public static void addObjects(FlatBufferBuilder builder, int objectsOffset) { builder.addOffset(0, objectsOffset, 0); }
  public static int createObjectsVector(FlatBufferBuilder builder, int[] data) { builder.startVector(4, data.length, 4); for (int i = data.length - 1; i >= 0; i--) builder.addOffset(data[i]); return builder.endVector(); }
  public static void startObjectsVector(FlatBufferBuilder builder, int numElems) { builder.startVector(4, numElems, 4); }
  public static int endObjectUpdated(FlatBufferBuilder builder) {
    int o = builder.endObject();
    return o;
  }
};

