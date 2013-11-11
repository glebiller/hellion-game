package fr.kissy.hellion.server.actor.creator;

import akka.actor.Actor;
import akka.japi.Creator;
import org.springframework.beans.factory.config.AutowireCapableBeanFactory;

/**
 * @author Guillaume Le Biller <lebiller@ekino.com>
 * @version $Id$
 */
public class SpringActorCreator<T extends Actor> implements Creator<T> {
    private AutowireCapableBeanFactory beanFactory;
    private Class<T> actorClass;

    /**
     * @inheritDoc
     */
    public SpringActorCreator(AutowireCapableBeanFactory beanFactory, Class<T> actorClass) {
        this.beanFactory = beanFactory;
        this.actorClass = actorClass;
    }

    /**
     * @inheritDoc
     */
    @Override
    public T create() throws Exception {
        return beanFactory.createBean(actorClass);
    }
}
