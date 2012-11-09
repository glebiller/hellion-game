package fr.kissy.hellion.server.config;

import akka.actor.ActorRef;
import akka.actor.ActorSystem;
import akka.actor.Props;
import fr.kissy.hellion.server.actor.AuthenticateActor;
import fr.kissy.hellion.server.handler.event.AuthenticatedMessageEvent;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;

@Configuration
public class ActorConfig {

    @Bean
    public ActorSystem actorSystem() {
        return ActorSystem.create("MainServer");
    }

    @Bean
    public ActorRef authenticateActor() {
        ActorRef actorRef = actorSystem().actorOf(new Props(AuthenticateActor.class));
        actorSystem().eventStream().subscribe(actorRef, AuthenticatedMessageEvent.class);
        return actorRef;
    }

}