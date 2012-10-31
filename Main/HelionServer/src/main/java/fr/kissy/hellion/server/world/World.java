package fr.kissy.hellion.server.world;

import com.infomatiq.jsi.rtree.RTree;
import fr.kissy.hellion.server.domain.Player;
import fr.kissy.hellion.server.handler.ServerPipelineFactory;
import org.jboss.netty.bootstrap.ServerBootstrap;
import org.jboss.netty.channel.socket.nio.NioServerSocketChannelFactory;

import java.net.InetSocketAddress;
import java.util.concurrent.Executors;

/**
 * @author Guillaume Le Biller <lebiller@ekino.com>
 * @version $Id$
 */
public class World {
    private static World INSTANCE;

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
     * Init the world.
     */
    public static void init() {
        INSTANCE = new World();

        // Bootstrap the server
        ServerBootstrap bootstrap = new ServerBootstrap(
                new NioServerSocketChannelFactory(Executors.newCachedThreadPool(), Executors.newCachedThreadPool())
        );
        bootstrap.setPipelineFactory(new ServerPipelineFactory());
        bootstrap.bind(new InetSocketAddress(9999));
    }

    /**
     * Get the world instance.
     *
     * @return The world instance.
     */
    public static World getInstance() {
        return INSTANCE;
    }

}
