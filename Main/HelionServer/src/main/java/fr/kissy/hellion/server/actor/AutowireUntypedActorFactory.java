package fr.kissy.hellion.server.actor;

import akka.actor.Actor;
import akka.actor.UntypedActorFactory;
import org.springframework.beans.factory.config.AutowireCapableBeanFactory;

/**
 * @author Guillaume Le Biller <lebiller@ekino.com>
 * @version $Id$
 */
public class AutowireUntypedActorFactory implements UntypedActorFactory {

    private AutowireCapableBeanFactory beanFactory;
    private Class<? extends Actor> actorClass;

    /**
     * @inheritDoc
     */
    public AutowireUntypedActorFactory(AutowireCapableBeanFactory beanFactory, Class<? extends Actor> actorClass) {
        this.beanFactory = beanFactory;
        this.actorClass = actorClass;
    }

    /**
     * @inheritDoc
     */
    @Override
    public Actor create() throws Exception {
        Actor actor = actorClass.newInstance();
        beanFactory.autowireBean(actor);
        return actor;
    }
}
