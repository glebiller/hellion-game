package fr.kissy.hellion.server.config;

import akka.actor.ActorRef;
import akka.actor.ActorSystem;
import akka.actor.Props;
import akka.event.Logging;
import akka.event.LoggingAdapter;
import akka.io.PipelineStage;
import akka.io.TcpPipelineHandler;
import akka.io.TcpReadWriteAdapter;
import com.typesafe.config.Config;
import com.typesafe.config.ConfigFactory;
import fr.kissy.hellion.proto.server.DownstreamMessageDto;
import fr.kissy.hellion.server.actor.AuthenticateActor;
import fr.kissy.hellion.server.actor.CreateEntityActor;
import fr.kissy.hellion.server.actor.DisconnectActor;
import fr.kissy.hellion.server.actor.PhysicActor;
import fr.kissy.hellion.server.actor.PlayerMoveActor;
import fr.kissy.hellion.server.actor.PlayerShotActor;
import fr.kissy.hellion.server.actor.ServerActor;
import fr.kissy.hellion.server.actor.SynchronizeActor;
import fr.kissy.hellion.server.actor.WorldActor;
import fr.kissy.hellion.server.actor.creator.SpringActorCreator;
import fr.kissy.hellion.server.actor.stage.ProtobufStage;
import fr.kissy.hellion.server.actor.stage.Varint32Stage;
import fr.kissy.hellion.server.bus.MessageEventBus;
import fr.kissy.hellion.server.bus.StateEventBus;
import fr.kissy.hellion.server.bus.event.EnumStateEventType;
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
        Config configFactory = ConfigFactory.load("fr/kissy/hellion/server/akka.conf");
        return ActorSystem.create("MainServer", configFactory);
    }

    @Bean
    public TcpPipelineHandler.Init tcpPipelineHandlerInit() {
        LoggingAdapter logger = Logging.getLogger(actorSystem(), serverActorRef());
        return TcpPipelineHandler.withLogger(logger,
                PipelineStage.sequence(new ProtobufStage(),
                        PipelineStage.sequence(new Varint32Stage(), new TcpReadWriteAdapter())
                )
        );
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
        ActorRef actorRef = actorSystem().actorOf(Props.create(new SpringActorCreator<AuthenticateActor>
                (beanFactory, AuthenticateActor.class)), AuthenticateActor.class.getSimpleName());
        messageEventBus().subscribe(actorRef, DownstreamMessageDto.DownstreamMessageProto.Type.AUTHENTICATE);
        return actorRef;
    }

    @Bean
    public ActorRef createEntityActorRef() {
        return actorSystem().actorOf(Props.create(new SpringActorCreator<CreateEntityActor>
                (beanFactory, CreateEntityActor.class)), CreateEntityActor.class.getSimpleName());
    }

    @Bean
    public ActorRef disconnectActorRef() {
        ActorRef actorRef = actorSystem().actorOf(Props.create(new SpringActorCreator<DisconnectActor>
                (beanFactory, DisconnectActor.class)), DisconnectActor.class.getSimpleName());
        stateEventBus().subscribe(actorRef, EnumStateEventType.DISCONNECTED);
        return actorRef;
    }

    @Bean
    public ActorRef physicActor() {
        return actorSystem().actorOf(Props.create(new SpringActorCreator<PhysicActor>
                (beanFactory, PhysicActor.class)).withDispatcher("physic-dispatcher"), PhysicActor.class.getSimpleName());
    }

    @Bean
    public ActorRef playerMoveActorRef() {
        ActorRef actorRef = actorSystem().actorOf(Props.create(new SpringActorCreator<PlayerMoveActor>
                (beanFactory, PlayerMoveActor.class)), PlayerMoveActor.class.getSimpleName());
        messageEventBus().subscribe(actorRef, DownstreamMessageDto.DownstreamMessageProto.Type.PLAYER_MOVE);
        return actorRef;
    }

    @Bean
    public ActorRef playerShotActorRef() {
        ActorRef actorRef = actorSystem().actorOf(Props.create(new SpringActorCreator<PlayerShotActor>
                (beanFactory, PlayerShotActor.class)), PlayerShotActor.class.getSimpleName());
        messageEventBus().subscribe(actorRef, DownstreamMessageDto.DownstreamMessageProto.Type.PLAYER_SHOT);
        return actorRef;
    }

    @Bean
    public ActorRef serverActorRef() {
        return actorSystem().actorOf(Props.create(new SpringActorCreator<ServerActor>
                (beanFactory, ServerActor.class)), ServerActor.class.getSimpleName());
    }

    @Bean
    @Qualifier("synchronizeActorRef")
    public ActorRef synchronizeActorRef() {
        ActorRef actorRef = actorSystem().actorOf(Props.create(new SpringActorCreator<SynchronizeActor>
                (beanFactory, SynchronizeActor.class)), SynchronizeActor.class.getSimpleName());
        messageEventBus().subscribe(actorRef, DownstreamMessageDto.DownstreamMessageProto.Type.ENTER_WORLD);
        return actorRef;
    }

    @Bean
    @Qualifier("worldActorRef")
    public ActorRef worldActorRef() {
        ActorRef actorRef = actorSystem().actorOf(Props.create(new SpringActorCreator<WorldActor>
                (beanFactory, WorldActor.class)), WorldActor.class.getSimpleName());
        //messageEventBus().subscribe(actorRef, DownstreamMessageDto.DownstreamMessageProto.Type.ENTER_WORLD);
        return actorRef;
    }
}