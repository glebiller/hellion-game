package fr.kissy.hellion.server.core.rtree.splitter;

import fr.kissy.hellion.server.core.rtree.model.BoxObject;
import fr.kissy.hellion.server.core.rtree.model.Node;

/**
 * Node Splitter interface.
 */
public interface NodeSplitter<T extends BoxObject> {

    /**
     * Split the given node.
     *
     * @param n The node to split.
     */
    void split(Node<T> n);

}
