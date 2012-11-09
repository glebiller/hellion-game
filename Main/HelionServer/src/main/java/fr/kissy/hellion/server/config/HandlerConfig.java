package fr.kissy.hellion.server.config;

import fr.kissy.hellion.server.handler.MessageServerHandler;
import fr.kissy.hellion.server.handler.SecurityServerHandler;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;

@Configuration
public class HandlerConfig {

    @Bean
    public MessageServerHandler messageServerHandler() {
        return new MessageServerHandler();
    }

    @Bean
    public SecurityServerHandler securityServerHandler() {
        return new SecurityServerHandler();
    }

}