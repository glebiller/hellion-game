package fr.kissy.hellion.server.handler.message;

import fr.kissy.hellion.proto.server.DownstreamMessageDto;
import fr.kissy.hellion.proto.server.UpstreamMessageDto;

/**
 * @author Guillaume Le Biller <lebiller@ekino.com>
 * @version $Id$
 */
public interface MessageHandler {

    /**
     * Process a message.
     *
     * @param message The message to process.
     * @return The message to respond or null to respond an error message.
     */
    UpstreamMessageDto.UpstreamMessageProto.Builder process(DownstreamMessageDto.DownstreamMessageProto message);

}
