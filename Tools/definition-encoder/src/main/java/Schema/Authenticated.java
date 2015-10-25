// automatically generated, do not modify

package Schema;

import java.nio.*;
import java.lang.*;
import java.util.*;
import com.google.flatbuffers.*;

public class Authenticated extends Table {
  public static Authenticated getRootAsAuthenticated(ByteBuffer _bb) { _bb.order(ByteOrder.LITTLE_ENDIAN); return (new Authenticated()).__init(_bb.getInt(_bb.position()) + _bb.position(), _bb); }
  public Authenticated __init(int _i, ByteBuffer _bb) { bb_pos = _i; bb = _bb; return this; }

  public Object players(int j) { return players(new Object(), j); }
  public Object players(Object obj, int j) { int o = __offset(4); return o != 0 ? obj.__init(__indirect(__vector(o) + j * 4), bb) : null; }
  public int playersLength() { int o = __offset(4); return o != 0 ? __vector_len(o) : 0; }
  public ByteBuffer playersAsByteBuffer() { return __vector_as_bytebuffer(4, 4); }

  public static int createAuthenticated(FlatBufferBuilder builder,
      int players) {
    builder.startObject(1);
    Authenticated.addPlayers(builder, players);
    return Authenticated.endAuthenticated(builder);
  }

  public static void startAuthenticated(FlatBufferBuilder builder) { builder.startObject(1); }
  public static void addPlayers(FlatBufferBuilder builder, int playersOffset) { builder.addOffset(0, playersOffset, 0); }
  public static int createPlayersVector(FlatBufferBuilder builder, int[] data) { builder.startVector(4, data.length, 4); for (int i = data.length - 1; i >= 0; i--) builder.addOffset(data[i]); return builder.endVector(); }
  public static void startPlayersVector(FlatBufferBuilder builder, int numElems) { builder.startVector(4, numElems, 4); }
  public static int endAuthenticated(FlatBufferBuilder builder) {
    int o = builder.endObject();
    return o;
  }
};

