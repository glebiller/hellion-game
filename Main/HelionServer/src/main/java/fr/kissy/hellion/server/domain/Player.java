package fr.kissy.hellion.server.domain;

import com.infomatiq.jsi.Rectangle;
import org.springframework.data.annotation.Id;

/**
 * @author Guillaume Le Biller <lebiller@ekino.com>
 * @version $Id$
 */
public class Player extends Rectangle {

    @Id
    private int id;

    public int getId() {
        return id;
    }

    public void setId(int id) {
        this.id = id;
    }
}
