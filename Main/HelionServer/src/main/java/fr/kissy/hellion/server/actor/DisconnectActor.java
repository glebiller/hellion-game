package fr.kissy.hellion.server.actor;

import akka.actor.UntypedActor;
import fr.kissy.hellion.server.domain.Player;
import fr.kissy.hellion.server.service.WorldService;
import fr.kissy.hellion.server.handler.event.AuthenticatedStateEvent;
import org.apache.shiro.subject.Subject;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;

/**
 * @author Guillaume Le Biller <lebiller@ekino.com>
 * @version $Id$
 */
public class DisconnectActor extends UntypedActor {

    private static final Logger LOGGER = LoggerFactory.getLogger(AuthenticateActor.class);

    @Autowired
    private WorldService world;

    @Override
    public void onReceive(Object o) throws Exception {
        AuthenticatedStateEvent stateEvent = (AuthenticatedStateEvent) o;
        Subject subject = stateEvent.getSubject();

        Player player = (Player) subject.getSession().getAttribute(Player.class.getSimpleName());
        if (player != null) {
            world.removePlayer(player);
            LOGGER.info("Removed player from World {}", player.getId());
        }

        if (subject.isAuthenticated()) {
            subject.logout();
        }
    }
}
