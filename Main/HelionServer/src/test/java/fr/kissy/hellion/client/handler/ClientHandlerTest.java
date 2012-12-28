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
package fr.kissy.hellion.client.handler;

import com.google.protobuf.ByteString;
import com.google.protobuf.InvalidProtocolBufferException;
import fr.kissy.hellion.proto.message.Authenticated;
import fr.kissy.hellion.proto.message.ObjectUpdated;
import fr.kissy.hellion.proto.server.DownstreamMessageDto;
import fr.kissy.hellion.proto.server.UpstreamMessageDto;
import org.jboss.netty.channel.Channel;
import org.jboss.netty.channel.ChannelHandlerContext;
import org.jboss.netty.channel.ChannelStateEvent;
import org.jboss.netty.channel.ExceptionEvent;
import org.jboss.netty.channel.MessageEvent;
import org.jboss.netty.channel.SimpleChannelUpstreamHandler;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import java.util.concurrent.Executors;
import java.util.concurrent.ScheduledExecutorService;
import java.util.concurrent.TimeUnit;

/**
 * Handler implementation for the echo client.  It initiates the ping-pong
 * traffic between the echo client and server by sending the first event to
 * the server.
 */
public class ClientHandlerTest extends SimpleChannelUpstreamHandler {

    private static final Logger LOGGER = LoggerFactory.getLogger(ClientHandlerTest.class.getName());

    private final String user;

    /**
     * Creates a client-side handler.
     */
    public ClientHandlerTest(String user) {
        this.user = user;
    }

    private static final ScheduledExecutorService WORKER = Executors.newSingleThreadScheduledExecutor();

    @Override
    public void channelConnected(ChannelHandlerContext context, ChannelStateEvent event) {
        DownstreamMessageDto.DownstreamMessageProto.Builder builder = DownstreamMessageDto.DownstreamMessageProto.newBuilder();
        builder.setType(DownstreamMessageDto.DownstreamMessageProto.Type.AUTHENTICATE);
        builder.setData(ByteString.copyFrom(user.getBytes()));
        event.getChannel().write(builder.build());
    }

    @Override
    public void messageReceived(ChannelHandlerContext context, MessageEvent event) {
        UpstreamMessageDto.UpstreamMessageProto message = (UpstreamMessageDto.UpstreamMessageProto) event.getMessage();
        LOGGER.debug("client : {}", message.getType());

        switch (message.getType()) {
            case AUTHENTICATED:
                try {
                    Authenticated.AuthenticatedProto authenticated = Authenticated.AuthenticatedProto.parseFrom(message.getData());
                    LOGGER.debug("Received {} players \n{}", authenticated.getPlayersList().size(), authenticated.getPlayersList());
                } catch (InvalidProtocolBufferException e) {
                    LOGGER.error("Cannot parse proto", e);
                }

                final Channel channel = event.getChannel();

                WORKER.schedule(new Runnable() {
                    public void run() {
                        DownstreamMessageDto.DownstreamMessageProto.Builder downstream2 = DownstreamMessageDto.DownstreamMessageProto.newBuilder();
                        downstream2.setType(DownstreamMessageDto.DownstreamMessageProto.Type.PLAYER_MOVE);
                        channel.write(downstream2.build());
                    }
                }, 5, TimeUnit.SECONDS);
                WORKER.schedule(new Runnable() {
                    public void run() {
                        DownstreamMessageDto.DownstreamMessageProto.Builder downstream2 = DownstreamMessageDto.DownstreamMessageProto.newBuilder();
                        downstream2.setType(DownstreamMessageDto.DownstreamMessageProto.Type.PLAYER_MOVE);
                        channel.write(downstream2.build());
                    }
                }, 15, TimeUnit.SECONDS);

                DownstreamMessageDto.DownstreamMessageProto.Builder downstream = DownstreamMessageDto.DownstreamMessageProto.newBuilder();
                downstream.setType(DownstreamMessageDto.DownstreamMessageProto.Type.ENTER_WORLD);
                event.getChannel().write(downstream.build());
                break;
            case OBJECT_CREATED:
            case OBJECT_DELETED:
            case OBJECT_UPDATED:
                try {
                    ObjectUpdated.ObjectUpdatedProto spawned = ObjectUpdated.ObjectUpdatedProto.parseFrom(message.getData());
                    LOGGER.debug("Received {} objects around \n{}", spawned.getObjectsList().size(), spawned.getObjectsList());
                } catch (InvalidProtocolBufferException e) {
                    LOGGER.error("Cannot parse proto", e);
                }
                break;
        }
    }

    @Override
    public void exceptionCaught(ChannelHandlerContext context, ExceptionEvent event) {
        // Close the connection when an exception is raised.
        LOGGER.warn("Unexpected exception from downstream.", event);
        event.getChannel().close();
    }
}
