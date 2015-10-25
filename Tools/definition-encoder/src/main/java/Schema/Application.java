// automatically generated, do not modify

package Schema;

import java.nio.*;
import java.lang.*;
import java.util.*;
import com.google.flatbuffers.*;

public class Application extends Table {
  public static Application getRootAsApplication(ByteBuffer _bb) { _bb.order(ByteOrder.LITTLE_ENDIAN); return (new Application()).__init(_bb.getInt(_bb.position()) + _bb.position(), _bb); }
  public Application __init(int _i, ByteBuffer _bb) { bb_pos = _i; bb = _bb; return this; }

  public String startupScene() { int o = __offset(4); return o != 0 ? __string(o + bb_pos) : null; }
  public ByteBuffer startupSceneAsByteBuffer() { return __vector_as_bytebuffer(4, 1); }
  public Property properties(int j) { return properties(new Property(), j); }
  public Property properties(Property obj, int j) { int o = __offset(6); return o != 0 ? obj.__init(__indirect(__vector(o) + j * 4), bb) : null; }
  public int propertiesLength() { int o = __offset(6); return o != 0 ? __vector_len(o) : 0; }
  public ByteBuffer propertiesAsByteBuffer() { return __vector_as_bytebuffer(6, 4); }
  public System systems(int j) { return systems(new System(), j); }
  public System systems(System obj, int j) { int o = __offset(8); return o != 0 ? obj.__init(__indirect(__vector(o) + j * 4), bb) : null; }
  public int systemsLength() { int o = __offset(8); return o != 0 ? __vector_len(o) : 0; }
  public ByteBuffer systemsAsByteBuffer() { return __vector_as_bytebuffer(8, 4); }
  public Systems systemsTest(int j) { return systemsTest(new Systems(), j); }
  public Systems systemsTest(Systems obj, int j) { int o = __offset(10); return o != 0 ? obj.__init(__indirect(__vector(o) + j * 4), bb) : null; }
  public int systemsTestLength() { int o = __offset(10); return o != 0 ? __vector_len(o) : 0; }
  public ByteBuffer systemsTestAsByteBuffer() { return __vector_as_bytebuffer(10, 4); }
  public String scenes(int j) { int o = __offset(12); return o != 0 ? __string(__vector(o) + j * 4) : null; }
  public int scenesLength() { int o = __offset(12); return o != 0 ? __vector_len(o) : 0; }
  public ByteBuffer scenesAsByteBuffer() { return __vector_as_bytebuffer(12, 4); }

  public static int createApplication(FlatBufferBuilder builder,
      int startupScene,
      int properties,
      int systems,
      int systemsTest,
      int scenes) {
    builder.startObject(5);
    Application.addScenes(builder, scenes);
    Application.addSystemsTest(builder, systemsTest);
    Application.addSystems(builder, systems);
    Application.addProperties(builder, properties);
    Application.addStartupScene(builder, startupScene);
    return Application.endApplication(builder);
  }

  public static void startApplication(FlatBufferBuilder builder) { builder.startObject(5); }
  public static void addStartupScene(FlatBufferBuilder builder, int startupSceneOffset) { builder.addOffset(0, startupSceneOffset, 0); }
  public static void addProperties(FlatBufferBuilder builder, int propertiesOffset) { builder.addOffset(1, propertiesOffset, 0); }
  public static int createPropertiesVector(FlatBufferBuilder builder, int[] data) { builder.startVector(4, data.length, 4); for (int i = data.length - 1; i >= 0; i--) builder.addOffset(data[i]); return builder.endVector(); }
  public static void startPropertiesVector(FlatBufferBuilder builder, int numElems) { builder.startVector(4, numElems, 4); }
  public static void addSystems(FlatBufferBuilder builder, int systemsOffset) { builder.addOffset(2, systemsOffset, 0); }
  public static int createSystemsVector(FlatBufferBuilder builder, int[] data) { builder.startVector(4, data.length, 4); for (int i = data.length - 1; i >= 0; i--) builder.addOffset(data[i]); return builder.endVector(); }
  public static void startSystemsVector(FlatBufferBuilder builder, int numElems) { builder.startVector(4, numElems, 4); }
  public static void addSystemsTest(FlatBufferBuilder builder, int systemsTestOffset) { builder.addOffset(3, systemsTestOffset, 0); }
  public static int createSystemsTestVector(FlatBufferBuilder builder, int[] data) { builder.startVector(4, data.length, 4); for (int i = data.length - 1; i >= 0; i--) builder.addOffset(data[i]); return builder.endVector(); }
  public static void startSystemsTestVector(FlatBufferBuilder builder, int numElems) { builder.startVector(4, numElems, 4); }
  public static void addScenes(FlatBufferBuilder builder, int scenesOffset) { builder.addOffset(4, scenesOffset, 0); }
  public static int createScenesVector(FlatBufferBuilder builder, int[] data) { builder.startVector(4, data.length, 4); for (int i = data.length - 1; i >= 0; i--) builder.addOffset(data[i]); return builder.endVector(); }
  public static void startScenesVector(FlatBufferBuilder builder, int numElems) { builder.startVector(4, numElems, 4); }
  public static int endApplication(FlatBufferBuilder builder) {
    int o = builder.endObject();
    return o;
  }
  public static void finishApplicationBuffer(FlatBufferBuilder builder, int offset) { builder.finish(offset); }
};

