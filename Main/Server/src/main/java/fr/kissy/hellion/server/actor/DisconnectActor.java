package fr.kissy.hellion.server.actor;

import akka.actor.UntypedActor;
import fr.kissy.hellion.proto.server.UpstreamMessageDto;
import fr.kissy.hellion.server.bus.event.StateEvent;
import fr.kissy.hellion.server.domain.Player;
import fr.kissy.hellion.server.service.UpstreamMessageService;
import fr.kissy.hellion.server.service.WorldService;
import org.apache.shiro.subject.Subject;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;

/**
 * @author Guillaume Le Biller <lebiller@ekino.com>
 * @version $Id$
 */
public class  DisconnectActor extends UntypedActor {
    private static final Logger LOGGER = LoggerFactory.getLogger(AuthenticateActor.class);

    @Autowired
    private WorldService world;
    @Autowired
    private UpstreamMessageService upstreamMessageService;

    @Override
    public void onReceive(Object o) throws Exception {
        StateEvent stateEvent = (StateEvent) o;
        Subject subject = stateEvent.getSubject();

        Player player = (Player) subject.getSession().getAttribute(Player.class.getSimpleName());
        if (player != null) {
            UpstreamMessageDto.UpstreamMessageProto playerDeleteMessage = upstreamMessageService.getObjectDeletedMessage(player);
            for (Player nearPlayer : player.getNearPlayers()) {
                nearPlayer.getNearPlayers().remove(player);
                nearPlayer.getSessionActor().tell(playerDeleteMessage, getSelf());
            }

            world.removePlayer(player);
            LOGGER.info("Removed player from World {}", player.getId());
        }

        if (subject.isAuthenticated()) {
            subject.logout();
        }
    }
}
