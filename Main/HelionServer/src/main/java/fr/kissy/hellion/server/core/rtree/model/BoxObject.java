package fr.kissy.hellion.server.core.rtree.model;

/**
 * An object bounded on an Axis-Aligned Bounding Box.
 *
 * @author Colonel32
 */
public interface BoxObject {

    /**
     * Get the Box for the object.
     *
     * @return The Box for the object.
     */
    Box getBox();

}