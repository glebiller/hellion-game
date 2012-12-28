package fr.kissy.hellion.server.core.rtree.model;

import fr.kissy.hellion.server.core.rtree.RTree;

import java.util.ArrayList;

/**
 * A node of the R-Tree.
 */
public class Node<T extends BoxObject> implements BoxObject {

    private Box box;
    private RTree rTree;
    private Node<T> parent;

    private ArrayList<Node<T>> children;
    private ArrayList<T> data;

    /**
     * Default constructor.
     *
     * @param rTree  The R-Tree that contains the node.
     * @param isLeaf True if the node is a leaf.
     */
    public Node(RTree<T> rTree, boolean isLeaf) {
        this.rTree = rTree;
        if (isLeaf) {
            data = new ArrayList<T>(rTree.getMaxSize() + 1);
        } else {
            children = new ArrayList<Node<T>>(rTree.getMaxSize() + 1);
        }
    }

    /**
     * Add the node to a parent node.
     *
     * @param parent The parent node.
     */
    @SuppressWarnings("unchecked")
    public void addTo(Node parent) {
        assert (parent.children != null);
        parent.children.add(this);
        this.parent = parent;
        computeMBR();
        rTree.getSplitter().split(parent);
    }

    /**
     * Compute the new MBR for the node.
     */
    public void computeMBR() {
        computeMBR(true);
    }

    /**
     * Compute the new MBR for the node.
     *
     * @param doParents True if we also compute parents.
     */
    @SuppressWarnings("unchecked")
    public void computeMBR(boolean doParents) {
        if (box == null) {
            box = new Box();
        }

        if (!isLeaf()) {
            if (children.isEmpty()) {
                return;
            }

            children.get(0).getBox().copyInto(box);
            for (int i = 1; i < children.size(); i++)
                box.merge(children.get(i).getBox());
        } else {
            if (data.isEmpty()) {
                return;
            }

            data.get(0).getBox().copyInto(box);
            for (int i = 1; i < data.size(); i++) {
                box.merge(data.get(i).getBox());
            }
        }

        if (doParents && parent != null) parent.computeMBR();
    }

    /**
     * Remove the node from the tree.
     */
    @SuppressWarnings("unchecked")
    public void remove() {
        if (parent == null) {
            assert (rTree.getRoot() == this);
            rTree.setRoot(null);
            return;
        }

        parent.children.remove(this);
        if (parent.children.isEmpty()) {
            parent.remove();
        } else {
            parent.computeMBR();
        }
    }

    /**
     * The size of the node.
     *
     * @return The size of the node.
     */
    public int size() {
        return isLeaf() ? data.size() : children.size();
    }

    /**
     * The depth of the node.
     *
     * @return The depth of the node.
     */
    public int depth() {
        Node n = this;
        int d = 0;
        while (n != null) {
            n = n.parent;
            d++;
        }
        return d;
    }

    /**
     * Is leaf ?
     *
     * @return Is leaf ?
     */
    public boolean isLeaf() {
        return data != null;
    }

    /**
     * Is root ?
     *
     * @return Is root ?
     */
    public boolean isRoot() {
        return parent == null;
    }

    public Box getBox() {
        return box;
    }

    public void setBox(Box box) {
        this.box = box;
    }

    public Node<T> getParent() {
        return parent;
    }

    public void setParent(Node<T> parent) {
        this.parent = parent;
    }

    public ArrayList<Node<T>> getChildren() {
        return children;
    }

    public ArrayList<T> getData() {
        return data;
    }

    /**
     * @ineheritDoc
     */
    @Override
    public String toString() {
        return "Depth: " + depth() + ", size: " + size();
    }
}
