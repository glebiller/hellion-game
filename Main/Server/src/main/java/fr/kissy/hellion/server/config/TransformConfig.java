package fr.kissy.hellion.server.config;

import fr.kissy.hellion.server.transform.PlayerToCreateObjectDto;
import fr.kissy.hellion.server.transform.PlayerToDeleteObjectDto;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;

/**
 * @author Guillaume Le Biller <lebiller@ekino.com>
 * @version $Id$
 */
@Configuration
public class TransformConfig {

    @Bean
    public PlayerToCreateObjectDto playerToObjectDto() {
        return new PlayerToCreateObjectDto();
    }

    @Bean
    public PlayerToDeleteObjectDto playerToDeleteObjectDto() {
        return new PlayerToDeleteObjectDto();
    }

}
