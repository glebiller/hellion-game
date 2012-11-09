package fr.kissy.hellion.server.config;

import fr.kissy.hellion.server.handler.ServerPipelineFactory;
import org.jboss.netty.bootstrap.ServerBootstrap;
import org.jboss.netty.channel.socket.nio.NioServerSocketChannelFactory;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;

import java.net.InetSocketAddress;

@Configuration
public class ServerConfig {

    @Value("${server.port}")
    private Integer serverPort;

    @Bean
    public ServerPipelineFactory serverPipelineFactory() {
        return new ServerPipelineFactory();
    }

    @Bean
    public ServerBootstrap serverBootstrap() {
        ServerBootstrap bootstrap = new ServerBootstrap(new NioServerSocketChannelFactory());
        bootstrap.setPipelineFactory(serverPipelineFactory());
        return bootstrap;
    }

    @Bean
    public InetSocketAddress inetSocketAddress() {
        return new InetSocketAddress(serverPort);
    }

}