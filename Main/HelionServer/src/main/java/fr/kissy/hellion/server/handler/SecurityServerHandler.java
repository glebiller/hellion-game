/*
 * Copyright 2012 The Netty Project
 *
 * The Netty Project licenses this file to you under the Apache License,
 * version 2.0 (the "License"); you may not use this file except in compliance
 * with the License. You may obtain a copy of the License at:
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
 * License for the specific language governing permissions and limitations
 * under the License.
 */
package fr.kissy.hellion.server.handler;

import fr.kissy.hellion.proto.server.DownstreamMessageDto;
import fr.kissy.hellion.proto.server.UpstreamMessageDto;
import fr.kissy.hellion.server.handler.event.AuthenticatedMessageEvent;
import fr.kissy.hellion.server.handler.event.AuthenticatedStateEvent;
import org.apache.shiro.SecurityUtils;
import org.apache.shiro.subject.Subject;
import org.jboss.netty.channel.ChannelHandlerContext;
import org.jboss.netty.channel.ChannelStateEvent;
import org.jboss.netty.channel.ExceptionEvent;
import org.jboss.netty.channel.MessageEvent;
import org.jboss.netty.channel.SimpleChannelUpstreamHandler;

import java.util.logging.Level;
import java.util.logging.Logger;

/**
 * Handler implementation for the echo server.
 */
public class SecurityServerHandler extends SimpleChannelUpstreamHandler {

    private static final Logger logger = Logger.getLogger(SecurityServerHandler.class.getName());

    /**
     * @inheritDoc
     */
    @Override
    public void messageReceived(ChannelHandlerContext context, MessageEvent event) {
        Subject subject = SecurityUtils.getSubject();
        DownstreamMessageDto.DownstreamMessageProto message = (DownstreamMessageDto.DownstreamMessageProto) event.getMessage();
        if (!subject.isAuthenticated() && message.getType() != DownstreamMessageDto.DownstreamMessageProto.Type.AUTHENTICATE) {
            UpstreamMessageDto.UpstreamMessageProto.Builder builder = UpstreamMessageDto.UpstreamMessageProto.newBuilder();
            builder.setType(UpstreamMessageDto.UpstreamMessageProto.Type.UNAUTHORIZED);
            event.getChannel().write(builder.build());
            return;
        }
        context.sendUpstream(new AuthenticatedMessageEvent(subject, event));
    }

    /**
     * @inheritDoc
     */
    @Override
    public void channelDisconnected(ChannelHandlerContext context, ChannelStateEvent event) throws Exception {
        Subject subject = SecurityUtils.getSubject();
        context.sendUpstream(new AuthenticatedStateEvent(subject, event));
    }

    /**
     * @inheritDoc
     */
    @Override
    public void exceptionCaught(ChannelHandlerContext context, ExceptionEvent event) {
        logger.log(Level.WARNING, "Unexpected exception from upstream.", event.getCause());
        event.getChannel().close();
    }

}
