package fr.kissy.hellion.server.actor;

import akka.actor.ActorSystem;
import akka.actor.UntypedActor;
import akka.dispatch.ExecutionContexts;
import akka.dispatch.Futures;
import com.google.common.collect.Sets;
import fr.kissy.hellion.proto.Message;
import fr.kissy.hellion.proto.server.UpstreamMessageDto;
import fr.kissy.hellion.server.bus.event.MessageEvent;
import fr.kissy.hellion.server.domain.Player;
import fr.kissy.hellion.server.service.PhysicService;
import fr.kissy.hellion.server.service.UpstreamMessageService;
import fr.kissy.hellion.server.service.WorldService;
import org.apache.shiro.config.Ini;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;
import scala.annotation.serializable;
import scala.concurrent.ExecutionContext;
import scala.concurrent.Future;
import scala.concurrent.duration.Duration;

import java.io.Serializable;
import java.util.Set;
import java.util.concurrent.Callable;
import java.util.concurrent.TimeUnit;


/**
 * Send the list of near players to be created & deleted.
 *
 * @author Guillaume Le Biller <lebiller@ekino.com>
 * @version $Id$
 */
public class PhysicActor extends UntypedActor {
    private static final Logger LOGGER = LoggerFactory.getLogger(PhysicActor.class);

    @Autowired
    private ActorSystem actorSystem;
    @Autowired
    private PhysicService physicService;
    private long last = System.nanoTime();

    @Override
    public void preStart() throws Exception {
        super.preStart();
        getSelf().tell(new Initialize(), getSelf());
    }

    @Override
    public void onReceive(Object o) throws Exception {
        /*if (o instanceof Initialize) {
            physicService.initialize();
            actorSystem.scheduler().schedule(Duration.Zero(), Duration.create(15, TimeUnit.MILLISECONDS),
                    getSelf(), new Step(), actorSystem.dispatcher(), getSelf());
        } else if (o instanceof AddEntity) {
            physicService.addEntity(null);
        } else if (o instanceof  Step) {
            double deltaTime = 1.d / 60.d;
            physicService.step(deltaTime);
            byte[] updates = physicService.entitiesUpdates();
            Message.ObjectUpdated objectUpdated = Message.ObjectUpdated.parseFrom(updates);
            System.out.println(objectUpdated);
        }*/
    }

    public static class Initialize implements Serializable {

    }
    public static class Step implements Serializable {

    }
    public static class AddEntity implements Serializable {

    }
}
