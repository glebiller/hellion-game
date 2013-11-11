package fr.kissy.hellion.server.actor.stage;

import akka.io.AbstractPipePair;
import akka.io.PipePair;
import akka.io.PipePairFactory;
import akka.io.PipelineContext;
import akka.io.PipelineStage;
import akka.util.ByteString;
import com.google.common.collect.Lists;
import com.google.protobuf.InvalidProtocolBufferException;
import fr.kissy.hellion.proto.server.DownstreamMessageDto;
import fr.kissy.hellion.proto.server.UpstreamMessageDto;
import scala.util.Either;

import java.util.ArrayList;

public class ProtobufStage extends PipelineStage<PipelineContext, UpstreamMessageDto.UpstreamMessageProto, ByteString, DownstreamMessageDto.DownstreamMessageProto, ByteString> {
    /**
     * @inheritDoc
     */
    @Override
    public PipePair<UpstreamMessageDto.UpstreamMessageProto, ByteString, DownstreamMessageDto.DownstreamMessageProto, ByteString> apply(final PipelineContext ctx) {
        return PipePairFactory.create(ctx, new AbstractPipePair<UpstreamMessageDto.UpstreamMessageProto, ByteString, DownstreamMessageDto.DownstreamMessageProto, ByteString>() {
            /**
             * @inheritDoc
             */
            @Override
            public Iterable<Either<DownstreamMessageDto.DownstreamMessageProto, ByteString>> onCommand(UpstreamMessageDto.UpstreamMessageProto cmd) {
                final ArrayList<Either<DownstreamMessageDto.DownstreamMessageProto, ByteString>> res = Lists.newArrayList();
                res.add(makeCommand(ByteString.fromArray(cmd.toByteArray())));
                return res;
            }

            /**
             * @inheritDoc
             */
            @Override
            public Iterable<Either<DownstreamMessageDto.DownstreamMessageProto, ByteString>> onEvent(ByteString event) {
                final ArrayList<Either<DownstreamMessageDto.DownstreamMessageProto, ByteString>> res = Lists.newArrayList();
                try {
                    res.add(makeEvent(DownstreamMessageDto.DownstreamMessageProto.getDefaultInstance()
                            .getParserForType().parseFrom(event.toArray())));
                } catch (InvalidProtocolBufferException e) {
                    e.printStackTrace();
                }
                return res;
            }
        });
    }
}