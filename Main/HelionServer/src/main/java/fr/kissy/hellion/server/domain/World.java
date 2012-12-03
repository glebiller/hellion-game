package fr.kissy.hellion.server.domain;

import com.infomatiq.jsi.rtree.RTree;

/**
 * @author Guillaume Le Biller <lebiller@ekino.com>
 * @version $Id$
 */
public class World {

    private RTree players = new RTree();

    /**
     * Default constructor
     */
    public World() {
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

    /**
     * Get the players.
     *
     * @return The players.
     */
    public RTree getPlayers() {
        return players;
    }
}
