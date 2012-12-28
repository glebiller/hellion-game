package fr.kissy.hellion.server.actor;

import akka.actor.UntypedActor;
import fr.kissy.hellion.proto.message.ObjectUpdated;
import fr.kissy.hellion.proto.server.UpstreamMessageDto;
import fr.kissy.hellion.server.domain.Player;
import fr.kissy.hellion.server.handler.event.AuthenticatedMessageEvent;
import fr.kissy.hellion.server.service.WorldService;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;

import java.util.List;


/**
 * Send the list of near players to be created & deleted.
 *
 * @author Guillaume Le Biller <lebiller@ekino.com>
 * @version $Id$
 */
public class SynchronizeActor extends UntypedActor {

    private static final Logger LOGGER = LoggerFactory.getLogger(SynchronizeActor.class);

    @Autowired
    private WorldService worldService;

    @Override
    public void onReceive(Object o) throws Exception {
        AuthenticatedMessageEvent messageEvent = (AuthenticatedMessageEvent) o;
        LOGGER.debug("Received event {} for user {}", messageEvent.getMessage().getType(), messageEvent.getSubject().getPrincipal());

        Player player = (Player) messageEvent.getSubject().getSession().getAttribute(Player.class.getSimpleName());

        ObjectUpdated.ObjectUpdatedProto.Builder createdObjects = ObjectUpdated.ObjectUpdatedProto.newBuilder();
        UpstreamMessageDto.UpstreamMessageProto.Builder objectCreated = UpstreamMessageDto.UpstreamMessageProto.newBuilder();
        objectCreated.setType(UpstreamMessageDto.UpstreamMessageProto.Type.OBJECT_CREATED);
        objectCreated.setData(createdObjects.build().toByteString());

        ObjectUpdated.ObjectUpdatedProto.Builder deletedObjects = ObjectUpdated.ObjectUpdatedProto.newBuilder();
        UpstreamMessageDto.UpstreamMessageProto.Builder objectDeleted = UpstreamMessageDto.UpstreamMessageProto.newBuilder();
        objectDeleted.setType(UpstreamMessageDto.UpstreamMessageProto.Type.OBJECT_DELETED);
        objectDeleted.setData(deletedObjects.build().toByteString());

        ObjectUpdated.ObjectUpdatedProto.Builder updateObject = ObjectUpdated.ObjectUpdatedProto.newBuilder();
        updateObject.addObjects(player.toObjectProtoBuilder());
        UpstreamMessageDto.UpstreamMessageProto.Builder objectUpdated = UpstreamMessageDto.UpstreamMessageProto.newBuilder();
        objectUpdated.setType(UpstreamMessageDto.UpstreamMessageProto.Type.OBJECT_UPDATED);
        objectUpdated.setData(updateObject.build().toByteString());

        UpstreamMessageDto.UpstreamMessageProto upstreamMessageProto = objectUpdated.build();

        List<Player> nearPlayers = worldService.getNearPlayers(player);
        LOGGER.debug("Number of near players for user {} is {}", player.getId(), nearPlayers.size());
        for (Player nearPlayer : nearPlayers) {
            if (player.hasLocalInstanceId(nearPlayer.getId())) {
                continue;
            }
            createdObjects.addObjects(nearPlayer.toObjectProtoBuilder());
            nearPlayer.getChannel().write(upstreamMessageProto);
        }

        // TODO build the delete list
        /*for (String id : player.getLocalInstanceIds()) {
            if (nearPlayers.contains(id))
        }*/

        player.getChannel().write(objectCreated.build());
        player.getChannel().write(objectDeleted.build());
    }
}
