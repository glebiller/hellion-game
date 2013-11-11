package fr.kissy.hellion.server.service;

import fr.kissy.hellion.server.domain.Player;
import fr.kissy.hellion.server.domain.World;
import org.springframework.beans.factory.annotation.Autowired;

import java.util.ArrayList;
import java.util.List;

/**
 * @author Guillaume Le Biller <lebiller@ekino.com>
 * @version $Id$
 */
public class WorldService {
    @Autowired
    private World world;

    /**
     * Add player to world.
     */
    public void addPlayer(Player player) {
        world.getPlayers().insert(player);
    }

    /**
     * Remove a player from world.
     *
     * @param player The player to remove.
     */
    public void removePlayer(Player player) {
        world.getPlayers().remove(player);
    }

    /**
     * Get the nearest Players.
     *
     * @param player The current Player.
     * @return The list of nearest players.
     */
    @SuppressWarnings("unchecked")
    public List<Player> getNearPlayers(Player player) {
        List<Player> nearestPlayers = new ArrayList<Player>();
        world.getPlayers().query(nearestPlayers, player.getNearestBounds(), player);
        return nearestPlayers;
    }

}
