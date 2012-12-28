package fr.kissy.hellion.server.domain;

import com.google.common.collect.Lists;
import com.google.protobuf.ByteString;
import fr.kissy.hellion.proto.common.ObjectDto;
import fr.kissy.hellion.proto.common.PropertyDto;
import fr.kissy.hellion.proto.common.SystemDto;
import fr.kissy.hellion.server.core.rtree.model.Box;
import fr.kissy.hellion.server.core.rtree.model.BoxObject;
import org.jboss.netty.channel.Channel;
import org.springframework.data.annotation.Id;

import java.beans.Transient;
import java.util.List;

/**
 * @author Guillaume Le Biller <lebiller@ekino.com>
 * @version $Id$
 */
public class Player implements BoxObject {

    @Id
    private String id;
    private int x;
    private int y;
    private int z;

    // Local Instance //
    private Channel channel;
    private List<String> localInstanceIds = Lists.newArrayList();

    // Protobuf Builder //
    private PropertyDto.PropertyProto.Builder positionProperty = PropertyDto.PropertyProto.newBuilder();
    private ObjectDto.ObjectProto.SystemObjectProto.Builder geometrySystemObject = ObjectDto.ObjectProto.SystemObjectProto.newBuilder();
    private ObjectDto.ObjectProto.Builder player = ObjectDto.ObjectProto.newBuilder();

    public Player() {
        positionProperty.setName("Position");
        positionProperty.addValue(ByteString.copyFromUtf8(String.valueOf(0)));
        positionProperty.addValue(ByteString.copyFromUtf8(String.valueOf(0)));
        positionProperty.addValue(ByteString.copyFromUtf8(String.valueOf(0)));

        geometrySystemObject.setSystemType(SystemDto.SystemProto.Type.Geometry);
        geometrySystemObject.addProperties(0, positionProperty);

        player.addSystemObjects(0, geometrySystemObject);
    }

    public String getId() {
        return id;
    }

    public void setId(String id) {
        this.id = id;
        player.setName(String.valueOf(id));
    }

    public int getX() {
        return x;
    }

    public void setX(int x) {
        this.x = x;
        positionProperty.setValue(0, ByteString.copyFromUtf8(String.valueOf(x)));
    }

    public int getY() {
        return y;
    }

    public void setY(int y) {
        this.y = y;
        positionProperty.setValue(1, ByteString.copyFromUtf8(String.valueOf(y)));
    }

    public int getZ() {
        return z;
    }

    public void setZ(int z) {
        this.z = z;
        positionProperty.setValue(2, ByteString.copyFromUtf8(String.valueOf(z)));
    }

    public void setPosition(int x, int y, int z) {
        setX(x);
        setY(y);
        setZ(z);
    }

    @Override
    public Box getBox() {
        return new Box(x, y, z);
    }

    @Transient
    public Box getNearestBounds() {
        Box nearest = getBox();
        nearest.expand(10, 10, 10);
        return nearest;
    }

    @Transient
    public boolean hasLocalInstanceId(String objectId) {
        return localInstanceIds.contains(objectId);
    }

    @Transient
    public List<String> getLocalInstanceIds() {
        return localInstanceIds;
    }

    @Transient
    public Channel getChannel() {
        return channel;
    }

    public void setChannel(Channel channel) {
        this.channel = channel;
    }

    @Transient
    public ObjectDto.ObjectProto.Builder toObjectProtoBuilder() {
        geometrySystemObject.setProperties(0, positionProperty);
        player.setSystemObjects(0, geometrySystemObject);
        return player;
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;

        Player player = (Player) o;
        return !(id != null ? !id.equals(player.id) : player.id != null);

    }

    @Override
    public int hashCode() {
        return id != null ? id.hashCode() : 0;
    }

}
