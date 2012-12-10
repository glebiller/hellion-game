package fr.kissy.hellion.server.core;

import java.util.ArrayList;

/**
* Created with IntelliJ IDEA.
* User: Guillaume
* Date: 10/12/12
* Time: 01:33
* To change this template use File | Settings | File Templates.
*/
public class Node<T extends BoundedObject> implements BoundedObject {
    private Node<T> parent;
    private AABB box;
    private ArrayList<Node<T>> children;
    private ArrayList<T> data;
    private RTree rTree;

    public Node(RTree<T> rTree) {
        this.rTree = rTree;
    }

    public Node(RTree<T> rTree, boolean isLeaf) {
        this.rTree = rTree;
        if (isLeaf)
            data = new ArrayList<T>(rTree.getMaxSize() + 1);
        else
            children = new ArrayList<Node<T>>(rTree.getMaxSize() + 1);
    }

    public boolean isLeaf() {
        return data != null;
    }

    public boolean isRoot() {
        return parent == null;
    }

    @SuppressWarnings("unchecked")
    public void addTo(Node parent) {
        assert (parent.children != null);
        parent.children.add(this);
        this.parent = parent;
        computeMBR();
        rTree.getSplitter().split(parent);
    }

    public void computeMBR() {
        computeMBR(true);
    }

    public void computeMBR(boolean doParents) {
        if (box == null) box = new AABB();
        if (!isLeaf()) {
            if (children.isEmpty()) return;
            children.get(0).box.copyInto(box);
            for (int i = 1; i < children.size(); i++)
                box.merge(children.get(i).box);
        } else {
            if (data.isEmpty()) return;
            data.get(0).getBounds().copyInto(box);
            for (int i = 1; i < data.size(); i++)
                box.merge(data.get(i).getBounds());
        }

        if (doParents && parent != null) parent.computeMBR();
    }

    public void remove() {
        if (parent == null) {
            assert (rTree.getRoot() == this);
            rTree.setRoot(null);
            return;
        }
        parent.children.remove(this);
        if (parent.children.isEmpty())
            parent.remove();
        else
            parent.computeMBR();
    }

    public ArrayList<? extends BoundedObject> getSubItems() {
        return isLeaf() ? data : children;
    }

    public AABB getBounds() {
        return box;
    }

    public boolean contains(int px, int py, int pz) {
        return box.contains(px, py, pz);
    }

    public int size() {
        return isLeaf() ? data.size() : children.size();
    }

    public int depth() {
        Node n = this;
        int d = 0;
        while (n != null) {
            n = n.parent;
            d++;
        }
        return d;
    }

    public String toString() {
        return "Depth: " + depth() + ", size: " + size();
    }

    public Node<T> getParent() {
        return parent;
    }

    public void setParent(Node<T> parent) {
        this.parent = parent;
    }

    public AABB getBox() {
        return box;
    }

    public void setBox(AABB box) {
        this.box = box;
    }

    public ArrayList<Node<T>> getChildren() {
        return children;
    }

    public void setChildren(ArrayList<Node<T>> children) {
        this.children = children;
    }

    public ArrayList<T> getData() {
        return data;
    }

    public void setData(ArrayList<T> data) {
        this.data = data;
    }

    public RTree getrTree() {
        return rTree;
    }

    public void setrTree(RTree rTree) {
        this.rTree = rTree;
    }
}
