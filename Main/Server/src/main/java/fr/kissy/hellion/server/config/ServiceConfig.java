package fr.kissy.hellion.server.config;

import fr.kissy.hellion.server.service.ObjectService;
import fr.kissy.hellion.server.service.PhysicService;
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
    public ObjectService objectService() {
        return new ObjectService();
    }
    @Bean
    public PhysicService physicService() {
        //System.load("D:\\My Documents\\Build\\hellion-game\\Debug\\Server.dll");
        return new PhysicService();
    }
    @Bean
    public UpstreamMessageService upstreamMessageService() {
        return new UpstreamMessageService();
    }
    @Bean
    public WorldService worldService() {
        return new WorldService();
    }
}
