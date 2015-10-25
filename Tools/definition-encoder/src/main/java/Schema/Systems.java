// automatically generated, do not modify

package Schema;

import java.nio.*;
import java.lang.*;
import java.util.*;
import com.google.flatbuffers.*;

public class Systems extends Table {
  public static Systems getRootAsSystems(ByteBuffer _bb) { _bb.order(ByteOrder.LITTLE_ENDIAN); return (new Systems()).__init(_bb.getInt(_bb.position()) + _bb.position(), _bb); }
  public Systems __init(int _i, ByteBuffer _bb) { bb_pos = _i; bb = _bb; return this; }

  public byte systemType() { int o = __offset(4); return o != 0 ? bb.get(o + bb_pos) : 0; }
  public Table system(Table obj) { int o = __offset(6); return o != 0 ? __union(obj, o) : null; }

  public static int createSystems(FlatBufferBuilder builder,
      byte system_type,
      int system) {
    builder.startObject(2);
    Systems.addSystem(builder, system);
    Systems.addSystemType(builder, system_type);
    return Systems.endSystems(builder);
  }

  public static void startSystems(FlatBufferBuilder builder) { builder.startObject(2); }
  public static void addSystemType(FlatBufferBuilder builder, byte systemType) { builder.addByte(0, systemType, 0); }
  public static void addSystem(FlatBufferBuilder builder, int systemOffset) { builder.addOffset(1, systemOffset, 0); }
  public static int endSystems(FlatBufferBuilder builder) {
    int o = builder.endObject();
    return o;
  }
};

