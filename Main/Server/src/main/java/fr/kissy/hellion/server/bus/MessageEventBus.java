package fr.kissy.hellion.server.bus;

import akka.actor.ActorRef;
import akka.event.japi.LookupEventBus;
import fr.kissy.hellion.proto.server.DownstreamMessageDto;
import fr.kissy.hellion.server.bus.event.MessageEvent;

public class MessageEventBus extends LookupEventBus<MessageEvent, ActorRef, DownstreamMessageDto.DownstreamMessageProto.Type> {

    /**
     * @inheritDoc
     */
    @Override
    public int mapSize() {
        return DownstreamMessageDto.DownstreamMessageProto.Type.values().length;
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
        return ((MessageEvent) o).getMessage().getType();
    }

    /**
     * @inheritDoc
     */
    @Override
    public void publish(Object o1, Object o2) {
        ((ActorRef) o2).tell(o1, null);
    }

}
