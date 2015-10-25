// automatically generated, do not modify

package Schema;

import java.nio.*;
import java.lang.*;
import java.util.*;
import com.google.flatbuffers.*;

public class PhysicSystem extends Table {
  public static PhysicSystem getRootAsPhysicSystem(ByteBuffer _bb) { _bb.order(ByteOrder.LITTLE_ENDIAN); return (new PhysicSystem()).__init(_bb.getInt(_bb.position()) + _bb.position(), _bb); }
  public PhysicSystem __init(int _i, ByteBuffer _bb) { bb_pos = _i; bb = _bb; return this; }

  public String namec() { int o = __offset(4); return o != 0 ? __string(o + bb_pos) : null; }
  public ByteBuffer namecAsByteBuffer() { return __vector_as_bytebuffer(4, 1); }

  public static int createPhysicSystem(FlatBufferBuilder builder,
      int namec) {
    builder.startObject(1);
    PhysicSystem.addNamec(builder, namec);
    return PhysicSystem.endPhysicSystem(builder);
  }

  public static void startPhysicSystem(FlatBufferBuilder builder) { builder.startObject(1); }
  public static void addNamec(FlatBufferBuilder builder, int namecOffset) { builder.addOffset(0, namecOffset, 0); }
  public static int endPhysicSystem(FlatBufferBuilder builder) {
    int o = builder.endObject();
    return o;
  }
};

