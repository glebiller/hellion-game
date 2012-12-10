package fr.kissy.hellion.server.core.rtree.splitter;

/**
 * Node splitting algorithms selectors.
 */
public enum SplitterType {
    /**
     * Quadratic splitting algorithm. Runs in O(n^2) time where n = maxChildren+1.
     * Use this for R-trees that update with new data fairly often.
     */
    QUADRATIC,
    /**
     * NOT IMPLEMENTED.
     * Exhaustive splitting algorithm. Runs in O(2^n) time where n = maxChildren+1.
     * Produces a more optimized result than the quadratic algorithm
     * at the cost of a high runtime. Use this for R-trees that update rarely
     * and queried frequently, such as zone protection.
     */
    EXHAUSTIVE,
}
