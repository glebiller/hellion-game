package fr.kissy.hellion.server.config;

import fr.kissy.hellion.server.service.WorldService;
import org.springframework.context.annotation.Bean;

/**
 * @author Guillaume Le Biller <lebiller@ekino.com>
 * @version $Id$
 */
public class ServiceConfig {

    @Bean
    public WorldService worldService() {
        return new WorldService();
    }

}
