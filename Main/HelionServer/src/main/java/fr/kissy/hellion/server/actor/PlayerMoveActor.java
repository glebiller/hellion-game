package fr.kissy.hellion.server.actor;

import akka.actor.ActorRef;
import akka.actor.UntypedActor;
import fr.kissy.hellion.proto.message.ObjectUpdated;
import fr.kissy.hellion.proto.server.UpstreamMessageDto;
import fr.kissy.hellion.server.domain.Player;
import fr.kissy.hellion.server.handler.event.AuthenticatedMessageEvent;
import fr.kissy.hellion.server.service.WorldService;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.beans.factory.annotation.Qualifier;

import java.util.List;


/**
 * @author Guillaume Le Biller <lebiller@ekino.com>
 * @version $Id$
 */
public class PlayerMoveActor extends UntypedActor {

    private static final Logger LOGGER = LoggerFactory.getLogger(PlayerMoveActor.class);

    @Autowired
    private WorldService worldService;
    @Autowired
    @Qualifier("synchronizeActorRef")
    private ActorRef synchronizeActorRef;

    @Override
    public void onReceive(Object o) throws Exception {
        AuthenticatedMessageEvent messageEvent = (AuthenticatedMessageEvent) o;
        LOGGER.debug("Received event {} for user {}", messageEvent.getMessage().getType(), messageEvent.getSubject().getPrincipal());

        Player player = (Player) messageEvent.getSubject().getSession().getAttribute(Player.class.getSimpleName());
        player.setX(player.getX() + 1);

        synchronizeActorRef.tell(o, getSelf());
    }

}
