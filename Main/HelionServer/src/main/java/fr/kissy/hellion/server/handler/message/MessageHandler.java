package fr.kissy.hellion.server.handler.message;

import fr.kissy.hellion.proto.DownstreamMessageDto;
import fr.kissy.hellion.proto.UpstreamMessageDto;

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
