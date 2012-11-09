package fr.kissy.hellion.server.handler.event;

import org.apache.shiro.subject.Subject;
import org.jboss.netty.channel.Channel;
import org.jboss.netty.channel.ChannelFuture;
import org.jboss.netty.channel.ChannelState;
import org.jboss.netty.channel.ChannelStateEvent;

import static org.jboss.netty.channel.Channels.succeededFuture;

/**
 * @author Guillaume Le Biller <lebiller@ekino.com>
 * @version $Id$
 */
public class AuthenticatedStateEvent implements ChannelStateEvent {

    private final Subject subject;
    private final Channel channel;
    private final ChannelState state;
    private final Object value;

    public AuthenticatedStateEvent(Subject subject, ChannelStateEvent channelStateEvent) {
        this.subject = subject;
        this.channel = channelStateEvent.getChannel();
        this.state = channelStateEvent.getState();
        this.value = channelStateEvent.getValue();
    }

    public Subject getSubject() {
        return subject;
    }

    @Override
    public Channel getChannel() {
        return channel;
    }

    @Override
    public ChannelFuture getFuture() {
        return succeededFuture(getChannel());
    }

    @Override
    public ChannelState getState() {
        return state;
    }

    @Override
    public Object getValue() {
        return value;
    }
}
