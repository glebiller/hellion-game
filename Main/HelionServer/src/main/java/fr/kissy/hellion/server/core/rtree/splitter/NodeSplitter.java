package fr.kissy.hellion.server.core.rtree.splitter;

import fr.kissy.hellion.server.core.rtree.model.BoundedObject;
import fr.kissy.hellion.server.core.rtree.model.Node;

/**
* Created with IntelliJ IDEA.
* User: Guillaume
* Date: 10/12/12
* Time: 01:35
* To change this template use File | Settings | File Templates.
*/
public interface NodeSplitter<T extends BoundedObject> {
    void split(Node<T> n);
}
