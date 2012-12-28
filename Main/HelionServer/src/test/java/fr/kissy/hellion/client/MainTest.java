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
package fr.kissy.hellion.client;

import fr.kissy.hellion.client.handler.ClientHandlerTest;
import fr.kissy.hellion.proto.server.UpstreamMessageDto;
import org.jboss.netty.bootstrap.ClientBootstrap;
import org.jboss.netty.channel.ChannelFuture;
import org.jboss.netty.channel.ChannelPipeline;
import org.jboss.netty.channel.ChannelPipelineFactory;
import org.jboss.netty.channel.Channels;
import org.jboss.netty.channel.socket.nio.NioClientSocketChannelFactory;
import org.jboss.netty.handler.codec.protobuf.ProtobufDecoder;
import org.jboss.netty.handler.codec.protobuf.ProtobufEncoder;
import org.jboss.netty.handler.codec.protobuf.ProtobufVarint32FrameDecoder;
import org.jboss.netty.handler.codec.protobuf.ProtobufVarint32LengthFieldPrepender;

import java.net.InetSocketAddress;

/**
 * Sends one event when a connection is open and echoes back any received
 * data to the server.  Simply put, the echo client initiates the ping-pong
 * traffic between the echo client and server by sending the first event to
 * the server.
 */
public class MainTest {

    private final String user;
    private final String host;
    private final int port;

    public MainTest(String user, String host, int port) {
        this.user = user;
        this.host = host;
        this.port = port;
    }

    public void run() {
        ClientBootstrap bootstrap = new ClientBootstrap(new NioClientSocketChannelFactory());
        bootstrap.setPipelineFactory(new ChannelPipelineFactory() {
            public ChannelPipeline getPipeline() throws Exception {
                return Channels.pipeline(
                        new ProtobufVarint32FrameDecoder(),
                        new ProtobufDecoder(UpstreamMessageDto.UpstreamMessageProto.getDefaultInstance()),
                        new ProtobufVarint32LengthFieldPrepender(),
                        new ProtobufEncoder(),
                        new ClientHandlerTest(user)
                );
            }
        });

        ChannelFuture future = bootstrap.connect(new InetSocketAddress(host, port));
        future.getChannel().getCloseFuture().awaitUninterruptibly();
        bootstrap.releaseExternalResources();
    }

    public static void main(String[] args) throws Exception {
        String user = (args.length >= 1) ? args[0] : "user1";
        new MainTest(user, "127.0.0.1", 9999).run();
    }
}
