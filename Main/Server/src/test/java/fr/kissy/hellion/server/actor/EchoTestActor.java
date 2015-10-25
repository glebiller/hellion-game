package fr.kissy.hellion.server.actor;

import akka.actor.ActorIdentity;
import akka.actor.ActorRef;
import akka.actor.ActorSelection;
import akka.actor.Identify;
import akka.actor.ReceiveTimeout;
import akka.actor.UntypedActor;
import com.google.protobuf.ByteString;
import fr.kissy.hellion.proto.server.UpstreamMessageDto;
import fr.kissy.hellion.server.bus.event.MessageEvent;
import fr.kissy.hellion.server.domain.Player;
import fr.kissy.hellion.server.service.UpstreamMessageService;
import fr.kissy.hellion.server.service.WorldService;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;

import java.util.List;


/**
 * @author Guillaume Le Biller <lebiller@ekino.com>
 * @version $Id$
 */
public class EchoTestActor extends UntypedActor {
    private static final Logger LOGGER = LoggerFactory.getLogger(EchoTestActor.class);
    private ActorRef remoteActor = null;

    public EchoTestActor() {
        sendIdentifyRequest();
    }

    private void sendIdentifyRequest() {
        getContext().actorSelection("akka.tcp://MainServer@127.0.0.1:2552/user/PhysicActor").tell(new Identify(1), getSelf());
    }

    @Override
    public void onReceive(Object message) throws Exception {
        LOGGER.info("Received event {}", message);
        if (message instanceof ActorIdentity) {
            remoteActor = ((ActorIdentity) message).getRef();
            PhysicActor.AddEntity addEntity = new PhysicActor.AddEntity();
            remoteActor.tell(addEntity, getSelf());
        } else if (message.equals(ReceiveTimeout.getInstance())) {
            sendIdentifyRequest();
        } else if (remoteActor == null) {
            System.out.println("Not ready yet");
        } else {
            LOGGER.info("Received event {}", message);
            unhandled(message);
        }
    }

}
