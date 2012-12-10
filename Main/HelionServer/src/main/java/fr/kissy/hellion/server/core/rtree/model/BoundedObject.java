package fr.kissy.hellion.server.core.rtree.model;

/**
 * An object bounded on an Axis-Aligned Bounding Box.
 *
 * @author Colonel32
 */
public interface BoundedObject {
    public AABB getBounds();
}