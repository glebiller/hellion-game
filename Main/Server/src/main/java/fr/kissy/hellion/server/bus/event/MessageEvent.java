package fr.kissy.hellion.server.bus.event;

import fr.kissy.hellion.proto.server.DownstreamMessageDto;
import org.apache.shiro.subject.Subject;

/**
 * @author Guillaume Le Biller <lebiller@ekino.com>
 * @version $Id$
 */
public class MessageEvent {
    private final Subject subject;
    private final DownstreamMessageDto.DownstreamMessageProto message;

    public MessageEvent(Subject subject, DownstreamMessageDto.DownstreamMessageProto message) {
        this.subject = subject;
        this.message = message;
    }

    public Subject getSubject() {
        return subject;
    }

    public DownstreamMessageDto.DownstreamMessageProto getMessage() {
        return message;
    }
}
