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

import fr.kissy.hellion.proto.DownstreamMessageDto;
import fr.kissy.hellion.proto.UpstreamMessageDto;
import fr.kissy.hellion.server.domain.Player;
import fr.kissy.hellion.server.handler.message.MessageHandler;
import fr.kissy.hellion.server.handler.message.impl.AuthenticateMessageHandler;
import fr.kissy.hellion.server.world.World;
import org.apache.shiro.SecurityUtils;
import org.apache.shiro.subject.Subject;
import org.jboss.netty.channel.ChannelHandlerContext;
import org.jboss.netty.channel.ChannelStateEvent;
import org.jboss.netty.channel.ExceptionEvent;
import org.jboss.netty.channel.MessageEvent;
import org.jboss.netty.channel.SimpleChannelUpstreamHandler;

import java.util.HashMap;
import java.util.Map;
import java.util.logging.Level;
import java.util.logging.Logger;

/**
 * Handler implementation for the echo server.
 */
public class MessageServerHandler extends SimpleChannelUpstreamHandler {

    private static final Logger LOGGER = Logger.getLogger(MessageServerHandler.class.getName());
    private static final Map<DownstreamMessageDto.DownstreamMessageProto.Type, MessageHandler> MESSAGE_HANDLERS
            = new HashMap<DownstreamMessageDto.DownstreamMessageProto.Type, MessageHandler>();

    public MessageServerHandler() {
        MESSAGE_HANDLERS.put(DownstreamMessageDto.DownstreamMessageProto.Type.AUTHENTICATE, new AuthenticateMessageHandler());
    }


    /**
     * @inheritDoc
     */
    @Override
    public void messageReceived(ChannelHandlerContext context, MessageEvent event) {
        DownstreamMessageDto.DownstreamMessageProto message = (DownstreamMessageDto.DownstreamMessageProto) event.getMessage();

        UpstreamMessageDto.UpstreamMessageProto.Builder builder = null;
        if (MESSAGE_HANDLERS.containsKey(message.getType())) {
            builder = MESSAGE_HANDLERS.get(message.getType()).process(message);
        }

        // If we are not able to process or the process is not good, then write an error message.
        // If the process went right, then the process method should have returned an answer.
        if (builder == null) {
            builder = UpstreamMessageDto.UpstreamMessageProto.newBuilder();
            builder.setType(UpstreamMessageDto.UpstreamMessageProto.Type.ERROR);
        }

        event.getChannel().write(builder.build());
    }

    /**
     * @inheritDoc
     */
    @Override
    public void channelDisconnected(ChannelHandlerContext ctx, ChannelStateEvent e) throws Exception {
        Subject subject = SecurityUtils.getSubject();
        if (subject.isAuthenticated()) {
            Player player = (Player) subject.getSession().getAttribute(Player.class.getSimpleName());
            if (player != null) {
                World.getInstance().removePlayer(player);
            }
        }
    }

    /**
     * @inheritDoc
     */
    @Override
    public void exceptionCaught(ChannelHandlerContext context, ExceptionEvent event) {
        LOGGER.log(Level.WARNING, "Unexpected exception from downstream.", event.getCause());
        event.getChannel().close();
    }

}
