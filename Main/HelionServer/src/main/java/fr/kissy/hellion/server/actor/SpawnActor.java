package fr.kissy.hellion.server.actor;

import akka.actor.UntypedActor;
import fr.kissy.hellion.proto.server.UpstreamMessageDto;
import fr.kissy.hellion.server.domain.Player;
import fr.kissy.hellion.server.domain.World;
import fr.kissy.hellion.server.handler.event.AuthenticatedMessageEvent;
import gnu.trove.TIntProcedure;
import org.apache.shiro.authc.AuthenticationException;
import org.apache.shiro.authc.UsernamePasswordToken;
import org.apache.shiro.subject.Subject;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;


/**
 * @author Guillaume Le Biller <lebiller@ekino.com>
 * @version $Id$
 */
public class SpawnActor extends UntypedActor {

    private static final Logger LOGGER = LoggerFactory.getLogger(SpawnActor.class);

    @Autowired
    private World world;

    @Override
    public void onReceive(Object o) throws Exception {
        AuthenticatedMessageEvent messageEvent = (AuthenticatedMessageEvent) o;
        LOGGER.debug("Received event {} for user {}", messageEvent.getMessage().getType(), messageEvent.getSubject().getPrincipal());

        Player player = (Player) messageEvent.getSubject().getSession().getAttribute(Player.class.getSimpleName());
        TIntProcedure procedure = new TIntProcedure() {
            @Override
            public boolean execute(int i) {
                LOGGER.info("Rectangle " + i);
                return false;
            }
        };
        world.getPlayers().nearest(player.centre(), procedure, 100f);
    }
}
