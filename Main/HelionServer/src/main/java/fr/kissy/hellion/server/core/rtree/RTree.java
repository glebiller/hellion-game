package fr.kissy.hellion.server.core.rtree;

import fr.kissy.hellion.server.core.rtree.model.AABB;
import fr.kissy.hellion.server.core.rtree.model.BoundedObject;
import fr.kissy.hellion.server.core.rtree.model.Node;
import fr.kissy.hellion.server.core.rtree.splitter.NodeSplitter;
import fr.kissy.hellion.server.core.rtree.splitter.impl.QuadraticNodeSplitter;
import fr.kissy.hellion.server.core.rtree.splitter.SplitterType;

import java.util.*;

/**
 * 3D R-Tree implementation for minecraft.
 * Uses algorithms from: http://www.sai.msu.su/~megera/postgres/gist/papers/Rstar3.pdf
 *
 * @author Colonel32
 */
public class RTree<T extends BoundedObject> {
    private Node<T> root;
    private int maxSize;
    private int minSize;
    private NodeSplitter<T> splitter;

    /**
     * Creates an R-Tree. Sets the splitting algorithm to quadratic splitting.
     *
     * @param minChildren Minimum children in a node.  {@code 2 <= minChildren <= maxChildren/2}
     * @param maxChildren Maximum children in a node. Node splits at this number + 1
     */
    public RTree(int minChildren, int maxChildren) {
        this(minChildren, maxChildren, SplitterType.QUADRATIC);
    }

    public RTree(int minChildren, int maxChildren, SplitterType splittertyp) {
        if (minChildren < 2 || minChildren > maxChildren / 2)
            throw new IllegalArgumentException("2 <= minChildren <= maxChildren/2");

        switch (splittertyp) {
            case QUADRATIC:
                splitter = new QuadraticNodeSplitter<T>(this);
                break;
            case EXHAUSTIVE:
                throw new UnsupportedOperationException("Not implemented yet.");
            default:
                throw new RuntimeException("Invalid node splitter");
        }

        this.minSize = minChildren;
        this.maxSize = maxChildren;
        root = null;
    }

    /**
     * Adds items whose AABB intersects the query AABB to results
     *
     * @param results A collection to store the query results
     * @param box     The query
     */
    public void query(Collection<T> results, AABB box) {
        query(results, box, root);
    }

    private void query(Collection<T> results, AABB box, Node<T> node) {
        if (node == null) return;
        if (node.isLeaf()) {
            for (int i = 0; i < node.getData().size(); i++) {
                T object = node.getData().get(i);
                if (object.getBounds().overlaps(box))
                    results.add(object);
            }
        } else {
            for (int i = 0; i < node.getChildren().size(); i++)
                if (node.getChildren().get(i).getBox().overlaps(box))
                    query(results, box, node.getChildren().get(i));
        }
    }

    /**
     * Returns one item that intersects the query box, or null if nothing intersects
     * the query box.
     */
    public T queryOne(AABB box) {
        return queryOne(box, root);
    }

    private T queryOne(AABB box, Node<T> node) {
        if (node == null) return null;
        if (node.isLeaf()) {
            for (int i = 0; i < node.getData().size(); i++) {
                T object = node.getData().get(i);
                if (object.getBounds().overlaps(box))
                    return object;
            }
            return null;
        } else {
            for (int i = 0; i < node.getChildren().size(); i++)
                if (node.getChildren().get(i).getBox().overlaps(box)) {
                    T result = queryOne(box, node.getChildren().get(i));
                    if (result != null) return result;
                }
            return null;
        }
    }

    /**
     * Adds items whose AABB contains the specified point.
     *
     * @param results A collection to store the query results.
     * @param px      Point X coordinate
     * @param py      Point Y coordinate
     * @param pz      Point Z coordinate
     */
    public void query(Collection<T> results, int px, int py, int pz) {
        query(results, px, py, pz, root);
    }

    private void query(Collection<T> results, int px, int py, int pz, Node<T> node) {
        if (node == null) return;
        if (node.isLeaf()) {
            for (int i = 0; i < node.getData().size(); i++) {
                T object = node.getData().get(i);
                if (object.getBounds().contains(px, py, pz))
                    results.add(object);
            }
        } else {
            for (int i = 0; i < node.getChildren().size(); i++)
                if (node.getChildren().get(i).getBox().contains(px, py, pz))
                    query(results, px, py, pz, node.getChildren().get(i));
        }
    }

    /**
     * Returns one item that intersects the query point, or null if no items intersect that point.
     */
    public T queryOne(int px, int py, int pz) {
        return queryOne(px, py, pz, root);
    }

    private T queryOne(int px, int py, int pz, Node<T> node) {
        if (node == null) return null;
        if (node.isLeaf()) {
            for (int i = 0; i < node.getData().size(); i++) {
                T object = node.getData().get(i);
                if (object.getBounds().contains(px, py, pz))
                    return object;
            }
            return null;
        } else {
            for (int i = 0; i < node.getChildren().size(); i++)
                if (node.getChildren().get(i).getBox().contains(px, py, pz)) {
                    T result = queryOne(px, py, pz, node.getChildren().get(i));
                    if (result != null) return result;
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
     * Inserts object o into the tree. Note that if the value of o.getAABB() changes
     * while in the R-tree, the result is undefined.
     *
     * @throws NullPointerException If o == null
     */
    public void insert(T o) {
        if (o == null) throw new NullPointerException("Cannot store null object");
        if (root == null)
            root = new Node<T>(this, true);

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
        if (root == null) return 0;
        return count(root);
    }

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

    private Node<T> chooseLeaf(T o, Node<T> n) {
        assert (n != null);
        if (n.isLeaf()) return n;
        else {
            AABB box = o.getBounds();

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
            if (maxnode == null) // Not sure how this could occur
                return null;
            return chooseLeaf(o, maxnode);
        }
    }

    public Node getRoot() {
        return root;
    }

    public void setRoot(Node<T> root) {
        this.root = root;
    }

    public int getMaxSize() {
        return maxSize;
    }

    public void setMaxSize(int maxSize) {
        this.maxSize = maxSize;
    }

    public int getMinSize() {
        return minSize;
    }

    public void setMinSize(int minSize) {
        this.minSize = minSize;
    }

    public NodeSplitter getSplitter() {
        return splitter;
    }

    public void setSplitter(NodeSplitter splitter) {
        this.splitter = splitter;
    }
}