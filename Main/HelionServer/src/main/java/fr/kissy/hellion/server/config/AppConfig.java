package fr.kissy.hellion.server.config;

import fr.kissy.hellion.server.domain.World;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;
import org.springframework.context.annotation.Import;
import org.springframework.context.annotation.ImportResource;

@Configuration
@ImportResource("classpath:/fr/kissy/hellion/server/properties-config.xml")
@Import({DatabaseConfig.class, SecurityConfig.class, ServerConfig.class, HandlerConfig.class, ActorConfig.class})
public class AppConfig {

    @Bean
    public World world() {
        return new World();
    }

}