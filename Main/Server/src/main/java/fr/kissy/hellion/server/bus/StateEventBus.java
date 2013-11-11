package fr.kissy.hellion.server.bus;

import akka.actor.ActorRef;
import akka.event.japi.LookupEventBus;
import akka.io.Tcp;
import fr.kissy.hellion.server.bus.event.EnumStateEventType;
import fr.kissy.hellion.server.bus.event.StateEvent;

public class StateEventBus extends LookupEventBus<StateEvent, ActorRef, EnumStateEventType> {
    /**
     * @inheritDoc
     */
    @Override
    public int mapSize() {
        /**
         * Tcp.ConnectionClosed
         */
        return 1;
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
        return ((StateEvent) o).getMessage();
    }

    /**
     * @inheritDoc
     */
    @Override
    public void publish(Object o1, Object o2) {
        ((ActorRef) o2).tell(o1, null);
    }

}
