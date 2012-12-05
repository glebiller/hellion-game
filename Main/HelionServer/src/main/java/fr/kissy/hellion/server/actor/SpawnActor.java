package fr.kissy.hellion.server.actor;

import akka.actor.UntypedActor;
import fr.kissy.hellion.proto.server.UpstreamMessageDto;
import fr.kissy.hellion.server.domain.Player;
import fr.kissy.hellion.server.handler.event.AuthenticatedMessageEvent;
import fr.kissy.hellion.server.service.WorldService;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;

import java.util.List;


/**
 * @author Guillaume Le Biller <lebiller@ekino.com>
 * @version $Id$
 */
public class SpawnActor extends UntypedActor {

    private static final Logger LOGGER = LoggerFactory.getLogger(SpawnActor.class);

    @Autowired
    private WorldService worldService;

    @Override
    public void onReceive(Object o) throws Exception {
        AuthenticatedMessageEvent messageEvent = (AuthenticatedMessageEvent) o;
        LOGGER.debug("Received event {} for user {}", messageEvent.getMessage().getType(), messageEvent.getSubject().getPrincipal());

        Player player = (Player) messageEvent.getSubject().getSession().getAttribute(Player.class.getSimpleName());
        List<Player> nearestPlayers = worldService.getNearest(player);

        LOGGER.info("Number of nearest players for user {} is {}", player.getId(), nearestPlayers.size());

        UpstreamMessageDto.UpstreamMessageProto.Builder builder = UpstreamMessageDto.UpstreamMessageProto.newBuilder();
        builder.setType(UpstreamMessageDto.UpstreamMessageProto.Type.AUTHENTICATED);
        builder.setData(player.getAuthenticatedData().build().toByteString());
        messageEvent.getChannel().write(builder.build());
    }
}
