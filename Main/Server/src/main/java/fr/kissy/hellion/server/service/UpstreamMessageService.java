package fr.kissy.hellion.server.service;

import com.google.common.collect.Iterables;
import com.google.common.collect.Sets;
import com.google.protobuf.ByteString;
import fr.kissy.hellion.proto.Message;
import fr.kissy.hellion.proto.server.UpstreamMessageDto;
import fr.kissy.hellion.server.domain.Player;
import fr.kissy.hellion.server.transform.PlayerToCreateObjectDto;
import fr.kissy.hellion.server.transform.PlayerToDeleteObjectDto;
import org.springframework.beans.factory.annotation.Autowired;

import java.util.Set;

/**
 * @author Guillaume LE BILLER
 */
public class UpstreamMessageService {
    @Autowired
    private PlayerToCreateObjectDto playerToCreateObjectDto;
    @Autowired
    private PlayerToDeleteObjectDto playerToDeleteObjectDto;
    @Autowired
    private ObjectService objectService;

    /**
     * Get the AUTHENTICATED UpstreamMessageProto
     * from the given player.
     *
     * @param player The given player to convert.
     * @return The created UpstreamMessageProto.
     */
    public UpstreamMessageDto.UpstreamMessageProto getAuthenticatedMessage(Player player) {
        Message.Authenticated.Builder dataBuilder = Message.Authenticated.newBuilder();
        dataBuilder.addPlayers(objectService.getCreateObject(player, true));

        UpstreamMessageDto.UpstreamMessageProto.Builder builder = UpstreamMessageDto.UpstreamMessageProto.newBuilder();
        builder.setType(UpstreamMessageDto.UpstreamMessageProto.Type.AUTHENTICATED);
        builder.setData(dataBuilder.build().toByteString());
        return builder.build();
    }

    /**
     * Get the OBJECT_CREATED UpstreamMessageProto
     * from the given player.
     *
     * @param player The player to convert.
     * @return The created UpstreamMessageProto.
     */
    public UpstreamMessageDto.UpstreamMessageProto getObjectCreatedMessage(Player player) {
        return getObjectCreatedMessage(Sets.newHashSet(player));
    }

    /**
     * Get the OBJECT_CREATED UpstreamMessageProto
     * from the Set of players.
     *
     * @param players The set of players to convert.
     * @return The created UpstreamMessageProto.
     */
    public UpstreamMessageDto.UpstreamMessageProto getObjectCreatedMessage(Set<Player> players) {
        Message.ObjectUpdated.Builder dataBuilder = Message.ObjectUpdated.newBuilder();
        dataBuilder.addAllObjects(Iterables.transform(players, playerToCreateObjectDto));
        return getObjectCreatedMessage(dataBuilder.build().toByteString());
    }

    /**
     * Get the OBJECT_CREATED UpstreamMessageProto
     * from the given player.
     *
     * @param data The data to send as ByteString.
     * @return The created UpstreamMessageProto.
     */
    public UpstreamMessageDto.UpstreamMessageProto getObjectCreatedMessage(ByteString data) {
        UpstreamMessageDto.UpstreamMessageProto.Builder builder = UpstreamMessageDto.UpstreamMessageProto.newBuilder();
        builder.setType(UpstreamMessageDto.UpstreamMessageProto.Type.OBJECT_CREATED);
        builder.setData(data);
        return builder.build();
    }

    /**
     * Get the OBJECT_UPDATED UpstreamMessageProto
     * from the given player.
     *
     * @param player The player to convert.
     * @return The created UpstreamMessageProto.
     */
    public UpstreamMessageDto.UpstreamMessageProto getObjectUpdatedMessage(Player player) {
        Message.ObjectUpdated.Builder dataBuilder = Message.ObjectUpdated.newBuilder();
        dataBuilder.addObjects(objectService.getUpdateObject(player));

        UpstreamMessageDto.UpstreamMessageProto.Builder builder = UpstreamMessageDto.UpstreamMessageProto.newBuilder();
        builder.setType(UpstreamMessageDto.UpstreamMessageProto.Type.OBJECT_UPDATED);
        builder.setData(dataBuilder.build().toByteString());
        return builder.build();
    }

    /**
     * Get the OBJECT_DELETED UpstreamMessageProto
     * from the Set of players.
     *
     * @param players The set of players to convert.
     * @return The created UpstreamMessageProto.
     */
    public UpstreamMessageDto.UpstreamMessageProto getObjectDeletedMessage(Set<Player> players) {
        Message.ObjectUpdated.Builder dataBuilder = Message.ObjectUpdated.newBuilder();
        dataBuilder.addAllObjects(Iterables.transform(players, playerToDeleteObjectDto));

        UpstreamMessageDto.UpstreamMessageProto.Builder builder = UpstreamMessageDto.UpstreamMessageProto.newBuilder();
        builder.setType(UpstreamMessageDto.UpstreamMessageProto.Type.OBJECT_DELETED);
        builder.setData(dataBuilder.build().toByteString());
        return builder.build();
    }

    /**
     * Get the OBJECT_DELETED UpstreamMessageProto
     * from the given player.
     *
     * @param player The given player to convert.
     * @return The created UpstreamMessageProto.
     */
    public UpstreamMessageDto.UpstreamMessageProto getObjectDeletedMessage(Player player) {
        return getObjectDeletedMessage(Sets.newHashSet(player));
    }

}
