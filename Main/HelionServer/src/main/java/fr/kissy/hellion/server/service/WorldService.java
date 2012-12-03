package fr.kissy.hellion.server.service;

import com.infomatiq.jsi.rtree.RTree;
import fr.kissy.hellion.server.domain.Player;

/**
 * @author Guillaume Le Biller <lebiller@ekino.com>
 * @version $Id$
 */
public class WorldService {

    private RTree players = new RTree();

    /**
     * Default constructor
     */
    public WorldService() {
        players.init(null);
    }

    /**
     * Add player to world.
     */
    public void addPlayer(Player player) {
        players.add(player, player.getId());
    }

    /**
     * Remove a player from world.
     *
     * @param player The player to remove.
     */
    public void removePlayer(Player player) {
        players.delete(player, player.getId());
    }

}
