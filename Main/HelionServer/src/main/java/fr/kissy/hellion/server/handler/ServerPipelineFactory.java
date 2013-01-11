package fr.kissy.hellion.server.handler;

import fr.kissy.hellion.proto.server.DownstreamMessageDto;
import org.jboss.netty.channel.ChannelHandler;
import org.jboss.netty.channel.ChannelPipeline;
import org.jboss.netty.channel.ChannelPipelineFactory;
import org.jboss.netty.channel.Channels;
import org.jboss.netty.handler.codec.frame.LengthFieldBasedFrameDecoder;
import org.jboss.netty.handler.codec.frame.LengthFieldPrepender;
import org.jboss.netty.handler.codec.protobuf.ProtobufDecoder;
import org.jboss.netty.handler.codec.protobuf.ProtobufEncoder;
import org.springframework.beans.factory.annotation.Autowired;

/**
 * @author Guillaume Le Biller <lebiller@ekino.com>
 * @version $Id$
 */
public class ServerPipelineFactory implements ChannelPipelineFactory {

    private static final int LENGHT_FIELD_SIZE = Integer.SIZE / 8;
    private static final ChannelHandler LENGHT_FIELD_DECODER = new LengthFieldBasedFrameDecoder(Integer.MAX_VALUE, 0, LENGHT_FIELD_SIZE, 0, LENGHT_FIELD_SIZE);
    private static final ChannelHandler LENGTH_FIELD_ENCODER = new LengthFieldPrepender(LENGHT_FIELD_SIZE);
    private static final ChannelHandler PROTOBUF_DECODER =  new ProtobufDecoder(DownstreamMessageDto.DownstreamMessageProto.getDefaultInstance());
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

        pipeline.addLast("frameDecoder", LENGHT_FIELD_DECODER);
        pipeline.addLast("protobufDecoder", PROTOBUF_DECODER);

        pipeline.addLast("frameEncoder", LENGTH_FIELD_ENCODER);
        pipeline.addLast("protobufEncoder", PROTOBUF_ENCODER);

        pipeline.addLast("security", securityServerHandler);
        pipeline.addLast("handler", messageServerHandler);

        return pipeline;
    }
}
