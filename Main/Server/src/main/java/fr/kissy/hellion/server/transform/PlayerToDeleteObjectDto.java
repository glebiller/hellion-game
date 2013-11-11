package fr.kissy.hellion.server.transform;

import com.google.common.base.Function;
import com.sun.istack.internal.Nullable;
import fr.kissy.hellion.proto.Common;
import fr.kissy.hellion.server.domain.Player;
import fr.kissy.hellion.server.service.ObjectService;
import org.springframework.beans.factory.annotation.Autowired;

/**
* @author Guillaume LE BILLER
*/
public class PlayerToDeleteObjectDto implements Function<Player, Common.Object> {

    @Autowired
    private ObjectService objectService;

    /**
     * @inheritDoc
     */
    @Override
    public Common.Object apply(@Nullable Player player) {
        return objectService.getDeleteObject(player);
    }
}
