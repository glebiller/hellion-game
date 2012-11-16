package fr.kissy.hellion.server.config;

import akka.actor.ActorRef;
import akka.actor.ActorSystem;
import akka.actor.Props;
import fr.kissy.hellion.server.actor.AuthenticateActor;
import fr.kissy.hellion.server.actor.AutowireUntypedActorFactory;
import fr.kissy.hellion.server.handler.event.AuthenticatedMessageEvent;
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
    public ActorRef authenticateActorRef() {
        ActorRef actorRef = actorSystem().actorOf(new Props(new AutowireUntypedActorFactory(beanFactory, AuthenticateActor.class)));
        actorSystem().eventStream().subscribe(actorRef, AuthenticatedMessageEvent.class);
        return actorRef;
    }

}