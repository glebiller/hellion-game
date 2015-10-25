package fr.kissy.hellion.server.actor;

import akka.actor.UntypedActor;
import com.google.protobuf.ByteString;
import fr.kissy.hellion.server.bus.event.MessageEvent;
import fr.kissy.hellion.server.domain.Player;
import org.bson.types.ObjectId;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;


/**
 * @author Guillaume Le Biller <lebiller@ekino.com>
 * @version $Id$
 */
public class CreateEntityActor extends UntypedActor {
    private static final Logger LOGGER = LoggerFactory.getLogger(CreateEntityActor.class);

    /**
     * @inheritDoc
     */
    @Override
    public void onReceive(Object o) throws Exception {
        MessageEvent messageEvent = (MessageEvent) o;
        LOGGER.debug("Received event {}", messageEvent.getMessage().getType());

        // TODO save a player to the database & load it
        Player player = new Player();
        player.setId(new ObjectId());
        player.setName("Npc 1");
        player.setPosition(
            ByteString.copyFrom(String.valueOf(500).getBytes()),
            ByteString.copyFrom(String.valueOf(20).getBytes()),
            ByteString.copyFrom(String.valueOf(500).getBytes())
        );

        LOGGER.info("Adding new player to World {}", player.getId());
    }
}
