package fr.kissy.hellion.server.actor;

import akka.actor.ActorRef;
import akka.actor.Props;
import akka.actor.UntypedActor;
import akka.io.Tcp;
import akka.io.TcpMessage;
import akka.io.TcpPipelineHandler;
import fr.kissy.hellion.server.actor.creator.SpringActorCreator;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.beans.factory.config.AutowireCapableBeanFactory;

import java.net.InetSocketAddress;

public class ServerActor extends UntypedActor {
    private static final Logger LOGGER = LoggerFactory.getLogger(ServerActor.class);

    @Value("${server.address}")
    private String serverAddress;
    @Value("${server.port}")
    private Integer serverPort;
    @Autowired
    private AutowireCapableBeanFactory beanFactory;
    @Autowired
    private TcpPipelineHandler.Init tcpPipelineHandlerInit;

    /**
     * @inheritDoc
     */
    @Override
    public void preStart() throws Exception {
        final ActorRef tcp = Tcp.get(getContext().system()).manager();
        tcp.tell(TcpMessage.bind(getSelf(), new InetSocketAddress(serverAddress, serverPort), 100), getSelf());
    }

    /**
     * @inheritDoc
     */
    @Override
    @SuppressWarnings("unchecked")
    public void onReceive(Object data) throws Exception {
        LOGGER.debug("Received server event {}", data);
        if (data instanceof Tcp.CommandFailed) {
            getContext().stop(getSelf());
        } else if (data instanceof Tcp.Connected) {
            final ActorRef handler = getContext().actorOf(Props.create(new SpringActorCreator<SessionActor>
                    (beanFactory, SessionActor.class))/*, SessionActor.class.getSimpleName()*/);
            final ActorRef pipeline = getContext().actorOf(TcpPipelineHandler.props(tcpPipelineHandlerInit,
                    getSender(), handler)/*, TcpPipelineHandler.class.getSimpleName()*/);
            getSender().tell(TcpMessage.register(pipeline), getSelf());
            handler.tell(pipeline, getSelf());
        }
    }

}