// automatically generated, do not modify

package Schema;

import java.nio.*;
import java.lang.*;
import java.util.*;
import com.google.flatbuffers.*;

public class GraphicSystem extends Table {
  public static GraphicSystem getRootAsGraphicSystem(ByteBuffer _bb) { _bb.order(ByteOrder.LITTLE_ENDIAN); return (new GraphicSystem()).__init(_bb.getInt(_bb.position()) + _bb.position(), _bb); }
  public GraphicSystem __init(int _i, ByteBuffer _bb) { bb_pos = _i; bb = _bb; return this; }

  public String namea() { int o = __offset(4); return o != 0 ? __string(o + bb_pos) : null; }
  public ByteBuffer nameaAsByteBuffer() { return __vector_as_bytebuffer(4, 1); }

  public static int createGraphicSystem(FlatBufferBuilder builder,
      int namea) {
    builder.startObject(1);
    GraphicSystem.addNamea(builder, namea);
    return GraphicSystem.endGraphicSystem(builder);
  }

  public static void startGraphicSystem(FlatBufferBuilder builder) { builder.startObject(1); }
  public static void addNamea(FlatBufferBuilder builder, int nameaOffset) { builder.addOffset(0, nameaOffset, 0); }
  public static int endGraphicSystem(FlatBufferBuilder builder) {
    int o = builder.endObject();
    return o;
  }
};

