package fr.kissy.hellion.server.actor;

import akka.actor.ActorRef;
import akka.actor.UntypedActor;
import akka.io.PipelineContext;
import akka.io.Tcp;
import akka.io.TcpPipelineHandler;
import fr.kissy.hellion.proto.server.DownstreamMessageDto;
import fr.kissy.hellion.proto.server.UpstreamMessageDto;
import fr.kissy.hellion.server.bus.MessageEventBus;
import fr.kissy.hellion.server.bus.StateEventBus;
import fr.kissy.hellion.server.bus.event.EnumStateEventType;
import fr.kissy.hellion.server.bus.event.MessageEvent;
import fr.kissy.hellion.server.bus.event.StateEvent;
import org.apache.shiro.subject.Subject;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;

public class SessionActor extends UntypedActor {
    private static final Logger LOGGER = LoggerFactory.getLogger(SessionActor.class);

    @Autowired
    private StateEventBus stateEventBus;
    @Autowired
    private MessageEventBus messageEventBus;
    @Autowired
    private TcpPipelineHandler.Init<PipelineContext, UpstreamMessageDto.UpstreamMessageProto,
            DownstreamMessageDto.DownstreamMessageProto> tcpPipelineHandlerInit;

    private Subject subject;
    private ActorRef tcpPipeline;

    public SessionActor() {
        this.subject = new Subject.Builder().buildSubject();
        this.subject.getSession(true).setAttribute(ActorRef.class.getSimpleName(), getSelf());
    }

    /**
     * @inheritDoc
     */
    @Override
    public void onReceive(Object data) throws Exception {
        LOGGER.debug("Received session event {} for user {}", data, subject.getPrincipal());
        if (data instanceof TcpPipelineHandler.Init.Event) {
            final DownstreamMessageDto.DownstreamMessageProto message = (DownstreamMessageDto.DownstreamMessageProto)
                    ((TcpPipelineHandler.Init.Event) data).evt();

            if (!subject.isAuthenticated() && message.getType() != DownstreamMessageDto.DownstreamMessageProto.Type.AUTHENTICATE) {
                UpstreamMessageDto.UpstreamMessageProto.Builder builder = UpstreamMessageDto.UpstreamMessageProto.newBuilder();
                builder.setType(UpstreamMessageDto.UpstreamMessageProto.Type.UNAUTHORIZED);
                getSelf().tell(builder.build(), getSelf());
                return;
            }

            messageEventBus.publish(new MessageEvent(subject, message));
        } else if (data instanceof UpstreamMessageDto.UpstreamMessageProto) {
            // TODO check if we could do this in a better way
            tcpPipeline.tell(tcpPipelineHandlerInit.command((UpstreamMessageDto.UpstreamMessageProto) data), getSelf());
        } else if (data instanceof ActorRef) {
            // TODO check if we could do this in a better way
            tcpPipeline = (ActorRef) data;
        } else if (data instanceof Tcp.ConnectionClosed) {
            getContext().stop(getSelf());
            stateEventBus.publish(new StateEvent(subject, EnumStateEventType.DISCONNECTED));
        }
    }
}