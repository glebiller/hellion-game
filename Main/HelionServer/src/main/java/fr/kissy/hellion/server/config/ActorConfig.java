package fr.kissy.hellion.server.config;

import akka.actor.ActorRef;
import akka.actor.ActorSystem;
import akka.actor.Props;
import akka.event.EventBus;
import fr.kissy.hellion.proto.server.DownstreamMessageDto;
import fr.kissy.hellion.server.actor.AuthenticateActor;
import fr.kissy.hellion.server.actor.AutowireUntypedActorFactory;
import fr.kissy.hellion.server.actor.SpawnActor;
import fr.kissy.hellion.server.config.bus.MessageEventBus;
import fr.kissy.hellion.server.config.bus.StateEventBus;
import fr.kissy.hellion.server.handler.event.AuthenticatedMessageEvent;
import org.jboss.netty.channel.MessageEvent;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.beans.factory.config.AutowireCapableBeanFactory;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;

@Configuration
public class ActorConfig {

    @Autowired
    private AutowireCapableBeanFactory beanFactory;

    @Bean(destroyMethod = "shutdown")
    public ActorSystem actorSystem() {
        return ActorSystem.create("MainServer");
    }

    @Bean
    public MessageEventBus messageEventBus() {
        return new MessageEventBus();
    }

    @Bean
    public StateEventBus stateEventBus() {
        return new StateEventBus();
    }

    @Bean
    public ActorRef authenticateActorRef() {
        ActorRef actorRef = actorSystem().actorOf(new Props(new AutowireUntypedActorFactory(beanFactory, AuthenticateActor.class)));
        messageEventBus().subscribe(actorRef, DownstreamMessageDto.DownstreamMessageProto.Type.AUTHENTICATE);
        return actorRef;
    }

    @Bean
    public ActorRef spawnActorRef() {
        ActorRef actorRef = actorSystem().actorOf(new Props(new AutowireUntypedActorFactory(beanFactory, SpawnActor.class)));
        messageEventBus().subscribe(actorRef, DownstreamMessageDto.DownstreamMessageProto.Type.SPAWN);
        return actorRef;
    }

}