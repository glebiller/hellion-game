package fr.kissy.hellion.server.domain;

import com.google.protobuf.ByteString;
import com.infomatiq.jsi.Rectangle;
import fr.kissy.hellion.proto.common.ObjectDto;
import fr.kissy.hellion.proto.common.PropertyDto;
import fr.kissy.hellion.proto.common.SystemDto;
import fr.kissy.hellion.proto.message.Authenticated;
import org.springframework.data.annotation.Id;

import java.beans.Transient;

/**
 * @author Guillaume Le Biller <lebiller@ekino.com>
 * @version $Id$
 */
public class Player extends Rectangle {

    @Id
    private int id;

    public int getId() {
        return id;
    }

    public void setId(int id) {
        this.id = id;
    }

    @Transient
    public void setPosition(float x, float y) {
        set(x, y, x, y);
    }

    @Transient
    private float getX() {
        return (minX + maxX) / 2;
    }

    @Transient
    private Object getY() {
        return (minY + maxY / 2);
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
        positionBuilder.addValue(ByteString.copyFromUtf8(String.valueOf(getX())));
        positionBuilder.addValue(ByteString.copyFromUtf8(String.valueOf(getY())));

        ObjectDto.ObjectProto.SystemObjectProto.Builder propertiesProto = ObjectDto.ObjectProto.SystemObjectProto.newBuilder();
        propertiesProto.setSystemType(SystemDto.SystemProto.Type.Geometry);
        propertiesProto.addProperties(positionBuilder);

        ObjectDto.ObjectProto.Builder builder = ObjectDto.ObjectProto.newBuilder();
        builder.setName(String.valueOf(id));
        builder.addSystemObjects(propertiesProto);
        return builder;
    }
}
