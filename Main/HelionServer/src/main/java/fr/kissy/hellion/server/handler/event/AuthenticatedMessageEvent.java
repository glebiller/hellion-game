package fr.kissy.hellion.server.handler.event;

import fr.kissy.hellion.proto.server.DownstreamMessageDto;
import org.apache.shiro.subject.Subject;
import org.jboss.netty.channel.Channel;
import org.jboss.netty.channel.ChannelFuture;
import org.jboss.netty.channel.MessageEvent;

import java.net.SocketAddress;

import static org.jboss.netty.channel.Channels.succeededFuture;

/**
 * @author Guillaume Le Biller <lebiller@ekino.com>
 * @version $Id$
 */
public class AuthenticatedMessageEvent implements MessageEvent {

    private final Subject subject;
    private final Channel channel;
    private final DownstreamMessageDto.DownstreamMessageProto message;
    private final SocketAddress remoteAddress;

    public AuthenticatedMessageEvent(Subject subject, MessageEvent messageEvent) {
        this.subject = subject;
        this.channel = messageEvent.getChannel();
        this.message = (DownstreamMessageDto.DownstreamMessageProto) messageEvent.getMessage();
        this.remoteAddress = messageEvent.getRemoteAddress();
    }

    public Subject getSubject() {
        return subject;
    }

    @Override
    public DownstreamMessageDto.DownstreamMessageProto getMessage() {
        return message;
    }

    @Override
    public SocketAddress getRemoteAddress() {
        return remoteAddress;
    }

    @Override
    public Channel getChannel() {
        return channel;
    }

    @Override
    public ChannelFuture getFuture() {
        return succeededFuture(getChannel());
    }
}
