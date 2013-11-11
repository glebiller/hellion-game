package fr.kissy.hellion.server.core.rtree.model;

/**
 * 3D Axis Aligned Bounding Box
 *
 * @author Colonel32
 */
public class Box {
    private int minx, miny, minz;
    private int maxx, maxy, maxz;

    public Box() {
        minx = miny = minz = 0;
        maxx = maxy = maxz = 0;
    }

    public Box(int x, int y, int z) {
        minx = maxx = x;
        miny = maxy = y;
        minz = maxz = z;
    }

    public Box getBounds() {
        return this;
    }

    public void setX(int x) {
        minx = x;
        maxx = x;
    }

    public int getX() {
        return (minx + maxx) / 2;
    }

    public void setY(int y) {
        miny = y;
        maxy = y;
    }

    public int getY() {
        return (miny + maxy) / 2;
    }

    public void setZ(int z) {
        minz = z;
        maxz = z;
    }

    public int getZ() {
        return (minz + maxz) / 2;
    }

    public void setMinCorner(int px, int py, int pz) {
        minx = px;
        miny = py;
        minz = pz;
    }

    public void setMaxCorner(int px, int py, int pz) {
        maxx = px;
        maxy = py;
        maxz = pz;
    }

    public void expand(int px, int py, int pz) {
        maxx += px;
        minx -= px;
        maxy += py;
        miny -= py;
        maxz += pz;
        minz -= pz;
    }

    public boolean contains(int px, int py, int pz) {
        return px >= minx && px <= maxx &&
                py >= miny && py <= maxy &&
                pz >= minz && pz <= maxz;
    }

    public boolean overlaps(Box other) {
        return minx <= other.maxx && maxx >= other.minx && miny <= other.maxy
                && maxy >= other.miny && minz <= other.maxz && maxz >= other.minz;
    }

    /**
     * Returns the amount of overlap between 2 AABBs. Result will be negative if they
     * do not overlap.
     */
    public int getOverlap(Box other) {
        int overlapx = (maxx - minx + other.maxx - other.minx) - Math.abs(minx + maxx - other.minx - other.minx);
        int overlapy = (maxy - miny + other.maxy - other.miny) - Math.abs(minx + maxy - other.miny - other.miny);
        int overlapz = (maxz - minz + other.maxz - other.minz) - Math.abs(minx + maxz - other.minz - other.minz);

        return Math.max(overlapx, Math.max(overlapy, overlapz));

    }

    /**
     * Returns the amount that other will need to be expanded to fit this.
     */
    public int expansionNeeded(Box other) {
        int total = 0;

        if (other.minx < minx) total += minx - other.minx;
        if (other.maxx > maxx) total += other.maxx - maxx;

        if (other.miny < miny) total += miny - other.miny;
        if (other.maxy > maxy) total += other.maxy - maxy;

        if (other.minz < minz) total += minz - other.minz;
        if (other.maxz > maxz) total += other.maxz - maxz;

        return total;
    }

    /**
     * Computes an AABB that contains both this and other and stores it in this.
     *
     * @return this
     */
    public Box merge(Box other) {
        minx = Math.min(minx, other.minx);
        maxx = Math.max(maxx, other.maxx);

        miny = Math.min(miny, other.miny);
        maxy = Math.max(maxy, other.maxy);

        minz = Math.min(minz, other.minz);
        maxz = Math.max(maxz, other.maxz);

        return this;
    }

    public int getVolume() {
        return (maxx - minx) * (maxy - miny) * (maxz - minz);
    }

    public Box getCopy() {
        Box clone = new Box();
        clone.minx = minx;
        clone.miny = miny;
        clone.minz = minz;

        clone.maxx = maxx;
        clone.maxy = maxy;
        clone.maxz = maxz;
        return clone;
    }

    public void copyInto(Box target) {
        target.minx = minx;
        target.miny = miny;
        target.minz = minz;

        target.maxx = maxx;
        target.maxy = maxy;
        target.maxz = maxz;
    }

    public boolean equals(Box other) {
        return minx == other.minx && maxx == other.maxx &&
                miny == other.miny && maxy == other.maxy &&
                minz == other.minz && maxz == other.maxz;
    }

    public String toString() {
        return String.format("(%1$d,%2$d,%3$d):(%4$d,%5$d,%6$d)", minx, miny, minz, maxx, maxy, maxz);
    }

}