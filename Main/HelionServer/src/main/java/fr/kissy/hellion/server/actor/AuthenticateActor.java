package fr.kissy.hellion.server.actor;

import akka.actor.UntypedActor;
import fr.kissy.hellion.proto.message.Authenticated;
import fr.kissy.hellion.proto.server.UpstreamMessageDto;
import fr.kissy.hellion.server.domain.Player;
import fr.kissy.hellion.server.handler.event.AuthenticatedMessageEvent;
import fr.kissy.hellion.server.service.WorldService;
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
public class AuthenticateActor extends UntypedActor {

    private static final Logger LOGGER = LoggerFactory.getLogger(AuthenticateActor.class);

    @Autowired
    private WorldService world;

    @Override
    public void onReceive(Object o) throws Exception {
        AuthenticatedMessageEvent messageEvent = (AuthenticatedMessageEvent) o;
        Subject subject = messageEvent.getSubject();
        LOGGER.debug("Received event {} for user {}", messageEvent.getMessage().getType(), subject.getPrincipal());

        if (subject.isAuthenticated()) {
            LOGGER.error("User {} is already authenticated", subject.getPrincipal());
            UpstreamMessageDto.UpstreamMessageProto.Builder builder = UpstreamMessageDto.UpstreamMessageProto.newBuilder();
            builder.setType(UpstreamMessageDto.UpstreamMessageProto.Type.UNAUTHORIZED);
            messageEvent.getChannel().write(builder.build());
            return;
        }

        String username = new String(messageEvent.getMessage().getData().toByteArray());
        UsernamePasswordToken usernamePasswordToken = new UsernamePasswordToken(username, "Test");
        LOGGER.debug("Authentication attempt for user {}", usernamePasswordToken.getUsername());
        try {
            subject.login(usernamePasswordToken);
        } catch (AuthenticationException e) {
            LOGGER.warn("Authentication failed for user {}", usernamePasswordToken.getUsername());
            UpstreamMessageDto.UpstreamMessageProto.Builder builder = UpstreamMessageDto.UpstreamMessageProto.newBuilder();
            builder.setType(UpstreamMessageDto.UpstreamMessageProto.Type.UNAUTHORIZED);
            messageEvent.getChannel().write(builder.build());
            return;
        }

        // TODO save a player to the database & load it
        // Fetch player & add it to world
        Player player = new Player();
        player.setId((String) subject.getPrincipal());
        player.setChannel(messageEvent.getChannel());
        subject.getSession().setAttribute(Player.class.getSimpleName(), player);
        world.addPlayer(player);

        LOGGER.info("Adding new player to World {}", player.getId());

        Authenticated.AuthenticatedProto.Builder authenticatedData = Authenticated.AuthenticatedProto.newBuilder();
        authenticatedData.addPlayers(player.toObjectProtoBuilder());

        UpstreamMessageDto.UpstreamMessageProto.Builder builder = UpstreamMessageDto.UpstreamMessageProto.newBuilder();
        builder.setType(UpstreamMessageDto.UpstreamMessageProto.Type.AUTHENTICATED);
        builder.setData(authenticatedData.build().toByteString());
        messageEvent.getChannel().write(builder.build());
    }
}
