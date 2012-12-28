package fr.kissy.hellion.server.config;

import akka.actor.ActorRef;
import akka.actor.ActorSystem;
import akka.actor.Props;
import fr.kissy.hellion.proto.server.DownstreamMessageDto;
import fr.kissy.hellion.server.actor.AuthenticateActor;
import fr.kissy.hellion.server.actor.AutowireUntypedActorFactory;
import fr.kissy.hellion.server.actor.DisconnectActor;
import fr.kissy.hellion.server.actor.PlayerMoveActor;
import fr.kissy.hellion.server.actor.SynchronizeActor;
import fr.kissy.hellion.server.config.bus.MessageEventBus;
import fr.kissy.hellion.server.config.bus.StateEventBus;
import org.jboss.netty.channel.ChannelState;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.beans.factory.annotation.Qualifier;
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
    public ActorRef disconnectActorRef() {
        ActorRef actorRef = actorSystem().actorOf(new Props(new AutowireUntypedActorFactory(beanFactory, DisconnectActor.class)));
        stateEventBus().subscribe(actorRef, ChannelState.CONNECTED);
        return actorRef;
    }

    @Bean
    @Qualifier("synchronizeActorRef")
    public ActorRef synchronizeActorRef() {
        ActorRef actorRef = actorSystem().actorOf(new Props(new AutowireUntypedActorFactory(beanFactory, SynchronizeActor.class)));
        messageEventBus().subscribe(actorRef, DownstreamMessageDto.DownstreamMessageProto.Type.ENTER_WORLD);
        return actorRef;
    }

    @Bean
    public ActorRef moveActorRef() {
        ActorRef actorRef = actorSystem().actorOf(new Props(new AutowireUntypedActorFactory(beanFactory, PlayerMoveActor.class)));
        messageEventBus().subscribe(actorRef, DownstreamMessageDto.DownstreamMessageProto.Type.PLAYER_MOVE);
        return actorRef;
    }

}