package fr.kissy.hellion.server.actor;

import akka.actor.ActorRef;
import akka.actor.UntypedActor;
import fr.kissy.hellion.proto.Common;
import fr.kissy.hellion.proto.Message;
import fr.kissy.hellion.server.bus.event.MessageEvent;
import fr.kissy.hellion.server.domain.Player;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.beans.factory.annotation.Qualifier;

import java.util.Arrays;


/**
 * @author Guillaume Le Biller <lebiller@ekino.com>
 * @version $Id$
 */
public class PlayerMoveActor extends UntypedActor {

    private static final Logger LOGGER = LoggerFactory.getLogger(PlayerMoveActor.class);

    @Autowired
    @Qualifier("synchronizeActorRef")
    private ActorRef synchronizeActorRef;

    @Override
    public void onReceive(Object o) throws Exception {
        MessageEvent messageEvent = (MessageEvent) o;
        LOGGER.debug("Received event {} for user {}", messageEvent.getMessage().getType(), messageEvent.getSubject().getPrincipal());

        Player player = (Player) messageEvent.getSubject().getSession().getAttribute(Player.class.getSimpleName());
        Message.ObjectUpdated updatedProto = Message.ObjectUpdated.parseFrom(messageEvent.getMessage().getData());
        for (Common.Property propertyProto : updatedProto.getObjects(0).getSystemObjects(0).getPropertiesList()) {
            if (propertyProto.getName().equals("Position")) {
                player.setPosition(
                    propertyProto.getValue(0),
                    propertyProto.getValue(1),
                    propertyProto.getValue(2)
                );
            } else if (propertyProto.getName().equals("Orientation")) {
                player.setOrientation(
                        propertyProto.getValue(0),
                        propertyProto.getValue(1),
                        propertyProto.getValue(2),
                        propertyProto.getValue(3)
                );
            } else if (propertyProto.getName().equals("Velocity")) {
                player.setVelocity(
                        propertyProto.getValue(0),
                        propertyProto.getValue(1),
                        propertyProto.getValue(2)
                );
            } else if (propertyProto.getName().equals("Rotation")) {
                player.setRotationProperty(
                    propertyProto.getValue(0),
                    propertyProto.getValue(1),
                    propertyProto.getValue(2)
                );
            }
        }

        LOGGER.debug("Received player move for user {}", messageEvent.getSubject().getPrincipal());
        synchronizeActorRef.tell(o, getSelf());
    }

}
