package fr.kissy.hellion.server.domain;

import akka.actor.ActorRef;
import com.google.common.collect.Sets;
import com.google.protobuf.ByteString;
import fr.kissy.hellion.proto.Common;
import fr.kissy.hellion.server.core.rtree.model.Box;
import fr.kissy.hellion.server.core.rtree.model.BoxObject;
import org.bson.types.ObjectId;
import org.springframework.data.annotation.Id;

import java.beans.Transient;
import java.util.Set;

/**
 * @author Guillaume Le Biller <lebiller@ekino.com>
 * @version $Id$
 */
public class Player implements BoxObject {

    @Id
    private ObjectId id;
    private String name;
    private Common.Property.Builder meshProperty;
    private Common.Property.Builder positionProperty;
    private Common.Property.Builder orientationProperty;
    private Common.Property.Builder velocityProperty;
    private Common.Property.Builder rotationProperty;

    // Local Instance
    private ActorRef sessionActor;
    private Set<Player> nearPlayers = Sets.newHashSet();

    public Player(String image) {
        meshProperty = Common.Property.newBuilder();
        meshProperty.setName("Mesh");
        meshProperty.addValue(ByteString.copyFrom(image.getBytes()));

        positionProperty = Common.Property.newBuilder();
        positionProperty.setName("Position");
        positionProperty.addValue(ByteString.copyFromUtf8(String.valueOf(0)));
        positionProperty.addValue(ByteString.copyFromUtf8(String.valueOf(0)));
        positionProperty.addValue(ByteString.copyFromUtf8(String.valueOf(0)));

        orientationProperty = Common.Property.newBuilder();
        orientationProperty.setName("Orientation");
        orientationProperty.addValue(ByteString.copyFromUtf8(String.valueOf(0)));
        orientationProperty.addValue(ByteString.copyFromUtf8(String.valueOf(0)));
        orientationProperty.addValue(ByteString.copyFromUtf8(String.valueOf(0)));
        orientationProperty.addValue(ByteString.copyFromUtf8(String.valueOf(1)));

        velocityProperty = Common.Property.newBuilder();
        velocityProperty.setName("Velocity");
        velocityProperty.addValue(ByteString.copyFromUtf8(String.valueOf(0)));
        velocityProperty.addValue(ByteString.copyFromUtf8(String.valueOf(0)));
        velocityProperty.addValue(ByteString.copyFromUtf8(String.valueOf(0)));

        rotationProperty = Common.Property.newBuilder();
        rotationProperty.setName("Rotation");
        rotationProperty.addValue(ByteString.copyFromUtf8(String.valueOf(0)));
        rotationProperty.addValue(ByteString.copyFromUtf8(String.valueOf(0)));
        rotationProperty.addValue(ByteString.copyFromUtf8(String.valueOf(0)));
    }

    public ObjectId getId() {
        return id;
    }

    public void setId(ObjectId id) {
        this.id = id;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public Common.Property.Builder getPositionProperty() {
        return positionProperty;
    }

    public Common.Property.Builder getMeshProperty() {
        return meshProperty;
    }

    public void setPosition(ByteString x, ByteString y, ByteString z) {
        positionProperty.setValue(0, x);
        positionProperty.setValue(1, y);
        positionProperty.setValue(2, z);
    }

    public Common.Property.Builder getOrientationProperty() {
        return orientationProperty;
    }

    public void setOrientation(ByteString x, ByteString y, ByteString z, ByteString w) {
        orientationProperty.setValue(0, x);
        orientationProperty.setValue(1, y);
        orientationProperty.setValue(2, z);
        orientationProperty.setValue(3, w);
    }

    public Common.Property.Builder getVelocityProperty() {
        return velocityProperty;
    }

    public void setVelocity(ByteString x, ByteString y, ByteString z) {
        velocityProperty.setValue(0, x);
        velocityProperty.setValue(1, y);
        velocityProperty.setValue(2, z);
    }

    public Common.Property.Builder getRotationProperty() {
        return rotationProperty;
    }

    public void setRotationProperty(ByteString x, ByteString y, ByteString z) {
        rotationProperty.setValue(0, x);
        rotationProperty.setValue(1, y);
        rotationProperty.setValue(2, z);
    }

    @Transient
    public ActorRef getSessionActor() {
        return sessionActor;
    }

    public void setSessionActor(ActorRef sessionActor) {
        this.sessionActor = sessionActor;
    }

    @Transient
    public Set<Player> getNearPlayers() {
        return nearPlayers;
    }

    @Override
    public Box getBox() {
        return new Box(
            Float.valueOf(new String(positionProperty.getValue(0).toByteArray())).intValue(),
            Float.valueOf(new String(positionProperty.getValue(1).toByteArray())).intValue(),
            Float.valueOf(new String(positionProperty.getValue(2).toByteArray())).intValue()
        );
    }

    @Transient
    public Box getNearestBounds() {
        Box nearest = getBox();
        nearest.expand(50, 50, 50);
        return nearest;
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;

        Player player = (Player) o;
        return id.equals(player.id);
    }

    @Override
    public int hashCode() {
        return id.hashCode();
    }

}
