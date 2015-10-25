package fr.kissy.hellion.server.actor;

import akka.actor.UntypedActor;
import com.google.common.collect.Lists;
import com.google.common.collect.Sets;
import fr.kissy.hellion.server.domain.Player;
import fr.kissy.hellion.server.domain.World;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;

import java.util.List;
import java.util.Set;

public class WorldActor extends UntypedActor {
    private static final Logger LOGGER = LoggerFactory.getLogger(WorldActor.class);

    @Autowired
    private World world;

    /**
     * @inheritDoc
     */
    @Override
    @SuppressWarnings("unchecked")
    public void onReceive(Object data) throws Exception {
        LOGGER.debug("Received world event {}", data);
        if (data instanceof AddPlayer) {
            world.getPlayers().insert(((AddPlayer) data).getPlayer());
        } else if (data instanceof  GetNearEntities) {
            Player player = ((GetNearEntities) data).getPlayer();
            NearEntitiesResponse nearEntitiesResponse = new NearEntitiesResponse(player);
            world.getPlayers().query(nearEntitiesResponse.getPlayers(), player.getNearestBounds(), player);
            getSender().tell(nearEntitiesResponse, getSelf());
        }
    }

    public class AddPlayer {
        private final Player player;

        public AddPlayer(Player player) {
            this.player = player;
        }

        public Player getPlayer() {
            return player;
        }
    }

    public class GetNearEntities {
        private final Player player;

        public GetNearEntities(Player player) {
            this.player = player;
        }

        public Player getPlayer() {
            return player;
        }
    }

    public class NearEntitiesResponse {
        private final Player player;
        private final Set<Player> players = Sets.newHashSet();

        public NearEntitiesResponse(Player player) {
            this.player = player;
        }

        public Player getPlayer() {
            return player;
        }

        public Set<Player> getPlayers() {
            return players;
        }
    }
}