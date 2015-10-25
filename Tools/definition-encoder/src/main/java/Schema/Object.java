// automatically generated, do not modify

package Schema;

import java.nio.*;
import java.lang.*;
import java.util.*;
import com.google.flatbuffers.*;

public class Object extends Table {
  public static Object getRootAsObject(ByteBuffer _bb) { _bb.order(ByteOrder.LITTLE_ENDIAN); return (new Object()).__init(_bb.getInt(_bb.position()) + _bb.position(), _bb); }
  public Object __init(int _i, ByteBuffer _bb) { bb_pos = _i; bb = _bb; return this; }

  public String id() { int o = __offset(4); return o != 0 ? __string(o + bb_pos) : null; }
  public ByteBuffer idAsByteBuffer() { return __vector_as_bytebuffer(4, 1); }
  public String name() { int o = __offset(6); return o != 0 ? __string(o + bb_pos) : null; }
  public ByteBuffer nameAsByteBuffer() { return __vector_as_bytebuffer(6, 1); }
  public String parent() { int o = __offset(8); return o != 0 ? __string(o + bb_pos) : null; }
  public ByteBuffer parentAsByteBuffer() { return __vector_as_bytebuffer(8, 1); }
  public String template() { int o = __offset(10); return o != 0 ? __string(o + bb_pos) : null; }
  public ByteBuffer templateAsByteBuffer() { return __vector_as_bytebuffer(10, 1); }
  public SystemObject systemObjects(int j) { return systemObjects(new SystemObject(), j); }
  public SystemObject systemObjects(SystemObject obj, int j) { int o = __offset(12); return o != 0 ? obj.__init(__indirect(__vector(o) + j * 4), bb) : null; }
  public int systemObjectsLength() { int o = __offset(12); return o != 0 ? __vector_len(o) : 0; }
  public ByteBuffer systemObjectsAsByteBuffer() { return __vector_as_bytebuffer(12, 4); }

  public static int createObject(FlatBufferBuilder builder,
      int id,
      int name,
      int parent,
      int template,
      int systemObjects) {
    builder.startObject(5);
    Object.addSystemObjects(builder, systemObjects);
    Object.addTemplate(builder, template);
    Object.addParent(builder, parent);
    Object.addName(builder, name);
    Object.addId(builder, id);
    return Object.endObject(builder);
  }

  public static void startObject(FlatBufferBuilder builder) { builder.startObject(5); }
  public static void addId(FlatBufferBuilder builder, int idOffset) { builder.addOffset(0, idOffset, 0); }
  public static void addName(FlatBufferBuilder builder, int nameOffset) { builder.addOffset(1, nameOffset, 0); }
  public static void addParent(FlatBufferBuilder builder, int parentOffset) { builder.addOffset(2, parentOffset, 0); }
  public static void addTemplate(FlatBufferBuilder builder, int templateOffset) { builder.addOffset(3, templateOffset, 0); }
  public static void addSystemObjects(FlatBufferBuilder builder, int systemObjectsOffset) { builder.addOffset(4, systemObjectsOffset, 0); }
  public static int createSystemObjectsVector(FlatBufferBuilder builder, int[] data) { builder.startVector(4, data.length, 4); for (int i = data.length - 1; i >= 0; i--) builder.addOffset(data[i]); return builder.endVector(); }
  public static void startSystemObjectsVector(FlatBufferBuilder builder, int numElems) { builder.startVector(4, numElems, 4); }
  public static int endObject(FlatBufferBuilder builder) {
    int o = builder.endObject();
    builder.required(o, 4);  // id
    return o;
  }
};

