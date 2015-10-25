package fr.kissy.hellion.server;

import akka.actor.ActorRef;
import akka.actor.ActorSystem;
import akka.actor.Props;
import com.typesafe.config.Config;
import com.typesafe.config.ConfigFactory;
import fr.kissy.hellion.server.actor.EchoTestActor;
import org.junit.Test;

/**
 * @author Guillaume Le Biller <lebiller@ekino.com>
 * @version $Id$
 */
public class SpawnObjectTest {
    public static void main(String[] args) {
        Config configFactory = ConfigFactory.load("fr/kissy/hellion/server/akka.test.conf");
        ActorSystem testServer = ActorSystem.create("testServer", configFactory);
        ActorRef actorRef = testServer.actorOf(Props.create(EchoTestActor.class), EchoTestActor.class.getSimpleName());
        //final ActorRef remoteActor = testServer.actorOf(Props.create(JSimpleCalculatorActor.class), JSimpleCalculatorActor.class.getSimpleName());
    }

    @Test
    public void testSpawnPlayer() throws InterruptedException {
        Config configFactory = ConfigFactory.load("fr/kissy/hellion/server/akka.test.conf");
        ActorSystem testServer = ActorSystem.create("testServer", configFactory);
        ActorRef actorRef = testServer.actorOf(Props.create(EchoTestActor.class), EchoTestActor.class.getSimpleName());
    }
}
