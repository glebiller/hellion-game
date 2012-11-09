package fr.kissy.hellion.server.handler;

import fr.kissy.hellion.proto.server.DownstreamMessageDto;
import org.jboss.netty.channel.ChannelHandler;
import org.jboss.netty.channel.ChannelPipeline;
import org.jboss.netty.channel.ChannelPipelineFactory;
import org.jboss.netty.channel.Channels;
import org.jboss.netty.handler.codec.protobuf.ProtobufDecoder;
import org.jboss.netty.handler.codec.protobuf.ProtobufEncoder;
import org.jboss.netty.handler.codec.protobuf.ProtobufVarint32FrameDecoder;
import org.jboss.netty.handler.codec.protobuf.ProtobufVarint32LengthFieldPrepender;
import org.springframework.beans.factory.annotation.Autowired;

/**
 * @author Guillaume Le Biller <lebiller@ekino.com>
 * @version $Id$
 */
public class ServerPipelineFactory implements ChannelPipelineFactory {

    private static final ChannelHandler PROTOBUF_FRAME_DECODER = new ProtobufVarint32FrameDecoder();
    private static final ChannelHandler PROTOBUF_DECODER =  new ProtobufDecoder(DownstreamMessageDto.DownstreamMessageProto.getDefaultInstance());
    private static final ChannelHandler PROTOBUF_FRAME_ENCODER = new ProtobufVarint32LengthFieldPrepender();
    private static final ChannelHandler PROTOBUF_ENCODER = new ProtobufEncoder();

    @Autowired
    private MessageServerHandler messageServerHandler;
    @Autowired
    private SecurityServerHandler securityServerHandler;

    /**
     * @inheritDoc
     */
    @Override
    public ChannelPipeline getPipeline() throws Exception {
        ChannelPipeline pipeline = Channels.pipeline();

        pipeline.addLast("frameDecoder", PROTOBUF_FRAME_DECODER);
        pipeline.addLast("protobufDecoder", PROTOBUF_DECODER);

        pipeline.addLast("frameEncoder", PROTOBUF_FRAME_ENCODER);
        pipeline.addLast("protobufEncoder", PROTOBUF_ENCODER);

        pipeline.addLast("security", securityServerHandler);
        pipeline.addLast("handler", messageServerHandler);

        return pipeline;
    }
}
