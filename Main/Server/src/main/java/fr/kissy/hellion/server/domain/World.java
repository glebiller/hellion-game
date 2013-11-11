package fr.kissy.hellion.server.domain;

import fr.kissy.hellion.server.core.rtree.RTree;
import org.springframework.beans.factory.annotation.Autowired;

/**
 * @author Guillaume Le Biller <lebiller@ekino.com>
 * @version $Id$
 */
public class World {

    @Autowired
    private RTree<Player> players;

    public RTree<Player> getPlayers() {
        return players;
    }
}
