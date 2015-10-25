package fr.kissy.hellion.server.service;

/**
 * @author Guillaume Le Biller (<i>lebiller@ekino.com</i>)
 * @version $Id$
 */
public class PhysicService {
    public native void initialize();
    public native void step(double deltaTime);
    public native byte[] entitiesUpdates();
    public native void addEntity(byte[] byteArray);
}
