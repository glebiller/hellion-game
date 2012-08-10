package fr.kissy.hellion.server.handler;

import fr.kissy.hellion.proto.DownstreamMessageDto;
import org.jboss.netty.channel.ChannelPipeline;
import org.jboss.netty.channel.ChannelPipelineFactory;
import org.jboss.netty.channel.Channels;
import org.jboss.netty.handler.codec.protobuf.ProtobufDecoder;
import org.jboss.netty.handler.codec.protobuf.ProtobufEncoder;
import org.jboss.netty.handler.codec.protobuf.ProtobufVarint32FrameDecoder;
import org.jboss.netty.handler.codec.protobuf.ProtobufVarint32LengthFieldPrepender;

/**
 * @author Guillaume Le Biller <lebiller@ekino.com>
 * @version $Id$
 */
public class ServerPipelineFactory implements ChannelPipelineFactory {
    /**
     * @inheritDoc
     */
    @Override
    public ChannelPipeline getPipeline() throws Exception {
        ChannelPipeline pipeline = Channels.pipeline();

        pipeline.addLast("frameDecoder", new ProtobufVarint32FrameDecoder());
        pipeline.addLast("protobufDecoder", new ProtobufDecoder(DownstreamMessageDto.DownstreamMessageProto.getDefaultInstance()));

        pipeline.addLast("frameEncoder", new ProtobufVarint32LengthFieldPrepender());
        pipeline.addLast("protobufEncoder", new ProtobufEncoder());

        pipeline.addLast("security", new SecurityServerHandler());
        pipeline.addLast("handler", new MessageServerHandler());
        return pipeline;
    }
}
