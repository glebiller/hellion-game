package fr.kissy.hellion.server.config.bus;

import akka.actor.ActorRef;
import akka.event.japi.LookupEventBus;
import fr.kissy.hellion.proto.server.DownstreamMessageDto;
import fr.kissy.hellion.server.handler.event.AuthenticatedMessageEvent;
import fr.kissy.hellion.server.handler.event.AuthenticatedStateEvent;
import org.jboss.netty.channel.ChannelState;

public class StateEventBus extends LookupEventBus<AuthenticatedStateEvent, ActorRef, ChannelState> {

    /**
     * @inheritDoc
     */
    @Override
    public int mapSize() {
        return ChannelState.values().length;
    }

    /**
     * @inheritDoc
     */
    @Override
    public int compareSubscribers(Object o1, Object o2) {
        return ((ActorRef) o1).compareTo((ActorRef) o2);
    }

    /**
     * @inheritDoc
     */
    @Override
    public Object classify(Object o) {
        return ((AuthenticatedStateEvent) o).getState();
    }

    /**
     * @inheritDoc
     */
    @Override
    public void publish(Object o1, Object o2) {
        ((ActorRef) o2).tell(o1, null);
    }

}
