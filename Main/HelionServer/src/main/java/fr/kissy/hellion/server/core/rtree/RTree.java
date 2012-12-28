package fr.kissy.hellion.server.core.rtree;

import fr.kissy.hellion.server.core.rtree.model.Box;
import fr.kissy.hellion.server.core.rtree.model.BoxObject;
import fr.kissy.hellion.server.core.rtree.model.Node;
import fr.kissy.hellion.server.core.rtree.splitter.NodeSplitter;
import fr.kissy.hellion.server.core.rtree.splitter.SplitterType;
import fr.kissy.hellion.server.core.rtree.splitter.impl.QuadraticNodeSplitter;

import java.util.Collection;

/**
 * 3D R-Tree implementation for minecraft.
 * Uses algorithms from: http://www.sai.msu.su/~megera/postgres/gist/papers/Rstar3.pdf
 *
 * @author Colonel32
 */
public class RTree<T extends BoxObject> {

    private NodeSplitter<T> splitter;
    private int maxSize;
    private int minSize;

    private Node<T> root;

    /**
     * Create an R-Tree with a given splitting algorithm.
     *
     * @param minChildren  Minimum children in a node.  {@code 2 <= minChildren <= maxChildren/2}
     * @param maxChildren  Maximum children in a node. Node splits at this number + 1
     * @param spliiterType The splitter type.
     */
    public RTree(int minChildren, int maxChildren, SplitterType spliiterType) {
        if (minChildren < 2 || minChildren > maxChildren / 2) {
            throw new IllegalArgumentException("2 <= minChildren <= maxChildren/2");
        }

        switch (spliiterType) {
            case QUADRATIC:
                splitter = new QuadraticNodeSplitter<T>(this);
                break;
            case EXHAUSTIVE:
                throw new UnsupportedOperationException("Not implemented yet.");
        }

        this.minSize = minChildren;
        this.maxSize = maxChildren;
        root = null;
    }

    /**
     * Adds items whose BoundingBox intersects the query BoundingBox to results
     *
     * @param results A collection to store the query results
     * @param box     The query box.
     * @param self    The current object to ignore in the results.
     */
    public void query(Collection<T> results, Box box, T self) {
        query(results, box, root, self);
    }

    /**
     * Recursive query for list of items that intersects with the query box.
     *
     * @param results A collection to store the query results
     * @param box     The query box.
     * @param node    The root node.
     * @param self    The self object in order to ignore it in the results.
     */
    private void query(Collection<T> results, Box box, Node<T> node, T self) {
        if (node == null) {
            return;
        }

        if (node.isLeaf()) {
            for (int i = 0; i < node.getData().size(); i++) {
                T object = node.getData().get(i);
                if (!object.equals(self) && object.getBox().overlaps(box)) {
                    results.add(object);
                }
            }
        } else {
            for (int i = 0; i < node.getChildren().size(); i++)
                if (node.getChildren().get(i).getBox().overlaps(box)) {
                    query(results, box, node.getChildren().get(i), self);
                }
        }
    }

    /**
     * Returns one item that intersects the query box, or null if nothing intersects
     * the query box.
     *
     * @param box The query box.
     */
    public T query(Box box) {
        return query(box, root);
    }

    /**
     * Recursive query for one item that intersects with the query box.
     *
     * @param box The query box.
     * @param node The starting node.
     * @return The object found.
     */
    private T query(Box box, Node<T> node) {
        if (node == null) {
            return null;
        }

        if (node.isLeaf()) {
            for (int i = 0; i < node.getData().size(); i++) {
                T object = node.getData().get(i);
                if (object.getBox().overlaps(box)) {
                    return object;
                }
            }
            return null;
        } else {
            for (int i = 0; i < node.getChildren().size(); i++)
                if (node.getChildren().get(i).getBox().overlaps(box)) {
                    T result = query(box, node.getChildren().get(i));
                    if (result != null) {
                        return result;
                    }
                }
            return null;
        }
    }

    /**
     * Removes the specified object if it is in the tree.
     *
     * @param o The object to remove.
     */
    public void remove(T o) {
        Node<T> n = chooseLeaf(o, root);
        assert (n.isLeaf());
        n.getData().remove(o);
        n.computeMBR();
    }

    /**
     * Inserts object o into the tree. Note that if the value of o.getBoundingBox() changes
     * while in the R-tree, the result is undefined.
     *
     * @throws NullPointerException If o == null
     */
    public void insert(T o) {
        if (o == null) {
            throw new NullPointerException("Cannot store null object");
        }

        if (root == null) {
            root = new Node<T>(this, true);
        }

        Node<T> n = chooseLeaf(o, root);
        assert (n.isLeaf());
        n.getData().add(o);
        n.computeMBR();
        splitter.split(n);
    }

    /**
     * Counts the number of items in the tree.
     */
    public int count() {
        if (root == null) {
            return 0;
        }
        return count(root);
    }

    /**
     * Recursif count.
     *
     * @param n The node object.
     * @return The sum of the given node object.
     */
    private int count(Node<T> n) {
        assert (n != null);
        if (n.isLeaf()) {
            return n.getData().size();
        } else {
            int sum = 0;
            for (int i = 0; i < n.getChildren().size(); i++)
                sum += count(n.getChildren().get(i));
            return sum;
        }
    }

    /**
     * Choose a leaf.
     *
     * @param o The object to start from.
     * @param n The node representing the object.
     * @return The found node that is a leaf.
     */
    private Node<T> chooseLeaf(T o, Node<T> n) {
        assert (n != null);
        if (n.isLeaf()) {
            return n;
        } else {
            Box box = o.getBox();

            int maxOverlap = Integer.MAX_VALUE;
            Node<T> maxnode = null;
            for (int i = 0; i < n.getChildren().size(); i++) {
                int overlap = n.getChildren().get(i).getBox().expansionNeeded(box);
                if ((overlap < maxOverlap) || (overlap == maxOverlap && maxnode != null
                        && n.getChildren().get(i).getBox().getVolume() < maxnode.getBox().getVolume())) {
                    maxOverlap = overlap;
                    maxnode = n.getChildren().get(i);
                }
            }
            if (maxnode == null) {
                return null;
            }
            return chooseLeaf(o, maxnode);
        }
    }

    public Node getRoot() {
        return root;
    }

    public void setRoot(Node<T> root) {
        this.root = root;
    }

    public NodeSplitter<T> getSplitter() {
        return splitter;
    }

    public int getMaxSize() {
        return maxSize;
    }

    public int getMinSize() {
        return minSize;
    }
}