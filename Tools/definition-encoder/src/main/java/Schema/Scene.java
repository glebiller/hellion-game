// automatically generated, do not modify

package Schema;

import java.nio.*;
import java.lang.*;
import java.util.*;
import com.google.flatbuffers.*;

public class Scene extends Table {
  public static Scene getRootAsScene(ByteBuffer _bb) { _bb.order(ByteOrder.LITTLE_ENDIAN); return (new Scene()).__init(_bb.getInt(_bb.position()) + _bb.position(), _bb); }
  public Scene __init(int _i, ByteBuffer _bb) { bb_pos = _i; bb = _bb; return this; }

  public String name() { int o = __offset(4); return o != 0 ? __string(o + bb_pos) : null; }
  public ByteBuffer nameAsByteBuffer() { return __vector_as_bytebuffer(4, 1); }
  public System systems(int j) { return systems(new System(), j); }
  public System systems(System obj, int j) { int o = __offset(6); return o != 0 ? obj.__init(__indirect(__vector(o) + j * 4), bb) : null; }
  public int systemsLength() { int o = __offset(6); return o != 0 ? __vector_len(o) : 0; }
  public ByteBuffer systemsAsByteBuffer() { return __vector_as_bytebuffer(6, 4); }
  public Object templates(int j) { return templates(new Object(), j); }
  public Object templates(Object obj, int j) { int o = __offset(8); return o != 0 ? obj.__init(__indirect(__vector(o) + j * 4), bb) : null; }
  public int templatesLength() { int o = __offset(8); return o != 0 ? __vector_len(o) : 0; }
  public ByteBuffer templatesAsByteBuffer() { return __vector_as_bytebuffer(8, 4); }
  public Object objects(int j) { return objects(new Object(), j); }
  public Object objects(Object obj, int j) { int o = __offset(10); return o != 0 ? obj.__init(__indirect(__vector(o) + j * 4), bb) : null; }
  public int objectsLength() { int o = __offset(10); return o != 0 ? __vector_len(o) : 0; }
  public ByteBuffer objectsAsByteBuffer() { return __vector_as_bytebuffer(10, 4); }
  public Link links(int j) { return links(new Link(), j); }
  public Link links(Link obj, int j) { int o = __offset(12); return o != 0 ? obj.__init(__indirect(__vector(o) + j * 4), bb) : null; }
  public int linksLength() { int o = __offset(12); return o != 0 ? __vector_len(o) : 0; }
  public ByteBuffer linksAsByteBuffer() { return __vector_as_bytebuffer(12, 4); }

  public static int createScene(FlatBufferBuilder builder,
      int name,
      int systems,
      int templates,
      int objects,
      int links) {
    builder.startObject(5);
    Scene.addLinks(builder, links);
    Scene.addObjects(builder, objects);
    Scene.addTemplates(builder, templates);
    Scene.addSystems(builder, systems);
    Scene.addName(builder, name);
    return Scene.endScene(builder);
  }

  public static void startScene(FlatBufferBuilder builder) { builder.startObject(5); }
  public static void addName(FlatBufferBuilder builder, int nameOffset) { builder.addOffset(0, nameOffset, 0); }
  public static void addSystems(FlatBufferBuilder builder, int systemsOffset) { builder.addOffset(1, systemsOffset, 0); }
  public static int createSystemsVector(FlatBufferBuilder builder, int[] data) { builder.startVector(4, data.length, 4); for (int i = data.length - 1; i >= 0; i--) builder.addOffset(data[i]); return builder.endVector(); }
  public static void startSystemsVector(FlatBufferBuilder builder, int numElems) { builder.startVector(4, numElems, 4); }
  public static void addTemplates(FlatBufferBuilder builder, int templatesOffset) { builder.addOffset(2, templatesOffset, 0); }
  public static int createTemplatesVector(FlatBufferBuilder builder, int[] data) { builder.startVector(4, data.length, 4); for (int i = data.length - 1; i >= 0; i--) builder.addOffset(data[i]); return builder.endVector(); }
  public static void startTemplatesVector(FlatBufferBuilder builder, int numElems) { builder.startVector(4, numElems, 4); }
  public static void addObjects(FlatBufferBuilder builder, int objectsOffset) { builder.addOffset(3, objectsOffset, 0); }
  public static int createObjectsVector(FlatBufferBuilder builder, int[] data) { builder.startVector(4, data.length, 4); for (int i = data.length - 1; i >= 0; i--) builder.addOffset(data[i]); return builder.endVector(); }
  public static void startObjectsVector(FlatBufferBuilder builder, int numElems) { builder.startVector(4, numElems, 4); }
  public static void addLinks(FlatBufferBuilder builder, int linksOffset) { builder.addOffset(4, linksOffset, 0); }
  public static int createLinksVector(FlatBufferBuilder builder, int[] data) { builder.startVector(4, data.length, 4); for (int i = data.length - 1; i >= 0; i--) builder.addOffset(data[i]); return builder.endVector(); }
  public static void startLinksVector(FlatBufferBuilder builder, int numElems) { builder.startVector(4, numElems, 4); }
  public static int endScene(FlatBufferBuilder builder) {
    int o = builder.endObject();
    builder.required(o, 4);  // name
    return o;
  }
};

