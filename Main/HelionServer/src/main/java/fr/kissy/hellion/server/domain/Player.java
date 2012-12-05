package fr.kissy.hellion.server.domain;

import com.google.protobuf.ByteString;
import fr.kissy.hellion.proto.common.ObjectDto;
import fr.kissy.hellion.proto.common.PropertyDto;
import fr.kissy.hellion.proto.common.SystemDto;
import fr.kissy.hellion.proto.message.Authenticated;
import fr.kissy.hellion.server.core.AABB;
import fr.kissy.hellion.server.core.BoundedObject;
import org.springframework.data.annotation.Id;

import java.beans.Transient;

/**
 * @author Guillaume Le Biller <lebiller@ekino.com>
 * @version $Id$
 */
public class Player implements BoundedObject {

    @Id
    private int id;
    private AABB aabb = new AABB();

    public int getId() {
        return id;
    }

    public void setId(int id) {
        this.id = id;
    }

    @Override
    public AABB getBounds() {
        return aabb;
    }

    public void setAabb(AABB aabb) {
        this.aabb = aabb;
    }

    public void setPosition(int x, int y, int z) {
        aabb.setX(x);
        aabb.setY(y);
        aabb.setZ(z);
    }

    @Transient
    public AABB getNearestBounds() {
        AABB nearest = aabb.getCopy();
        nearest.expand(10, 10, 10);
        return nearest;
    }

    @Transient
    public Authenticated.AuthenticatedProto.Builder getAuthenticatedData() {
        Authenticated.AuthenticatedProto.Builder builder = Authenticated.AuthenticatedProto.newBuilder();
        builder.setPlayer(toObjectProtoBuilder());
        return builder;
    }

    @Transient
    private ObjectDto.ObjectProto.Builder toObjectProtoBuilder() {
        PropertyDto.PropertyProto.Builder positionBuilder = PropertyDto.PropertyProto.newBuilder();
        positionBuilder.setName("Position");
        positionBuilder.addValue(ByteString.copyFromUtf8(String.valueOf(aabb.getX())));
        positionBuilder.addValue(ByteString.copyFromUtf8(String.valueOf(aabb.getY())));
        positionBuilder.addValue(ByteString.copyFromUtf8(String.valueOf(aabb.getZ())));

        ObjectDto.ObjectProto.SystemObjectProto.Builder propertiesProto = ObjectDto.ObjectProto.SystemObjectProto.newBuilder();
        propertiesProto.setSystemType(SystemDto.SystemProto.Type.Geometry);
        propertiesProto.addProperties(positionBuilder);

        ObjectDto.ObjectProto.Builder builder = ObjectDto.ObjectProto.newBuilder();
        builder.setName(String.valueOf(id));
        builder.addSystemObjects(propertiesProto);
        return builder;
    }
}
