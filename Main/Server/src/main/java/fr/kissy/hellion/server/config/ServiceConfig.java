package fr.kissy.hellion.server.config;

import fr.kissy.hellion.server.service.ObjectService;
import fr.kissy.hellion.server.service.UpstreamMessageService;
import fr.kissy.hellion.server.service.WorldService;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;

/**
 * @author Guillaume Le Biller <lebiller@ekino.com>
 * @version $Id$
 */
@Configuration
public class ServiceConfig {

    @Bean
    public WorldService worldService() {
        return new WorldService();
    }

    @Bean
    public ObjectService objectService() {
        return new ObjectService();
    }

    @Bean
    public UpstreamMessageService upstreamMessageService() {
        return new UpstreamMessageService();
    }

}
