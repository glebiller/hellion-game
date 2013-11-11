package fr.kissy.hellion.server.bus.event;

import org.apache.shiro.subject.Subject;

/**
 * @author Guillaume Le Biller <lebiller@ekino.com>
 * @version $Id$
 */
public class StateEvent {
    private final Subject subject;
    private final EnumStateEventType message;

    public StateEvent(Subject subject, EnumStateEventType message) {
        this.subject = subject;
        this.message = message;
    }

    public Subject getSubject() {
        return subject;
    }

    public EnumStateEventType getMessage() {
        return message;
    }
}
