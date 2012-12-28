package fr.kissy.hellion.server.config;

import fr.kissy.hellion.server.core.rtree.RTree;
import fr.kissy.hellion.server.core.rtree.splitter.SplitterType;
import fr.kissy.hellion.server.domain.World;
import fr.kissy.hellion.server.service.WorldService;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;
import org.springframework.context.annotation.Import;
import org.springframework.context.annotation.ImportResource;

@Configuration
@ImportResource("classpath:/fr/kissy/hellion/server/properties-config.xml")
@Import({DatabaseConfig.class, SecurityConfig.class, ServerConfig.class, HandlerConfig.class, ActorConfig.class, ServiceConfig.class})
public class AppConfig {

    @Bean
    public World world() {
        return new World();
    }

    @Bean
    public WorldService worldService() {
        return new WorldService();
    }

    @Bean
    public RTree rTree() {
        return new RTree(15, 30, SplitterType.QUADRATIC);
    }

}