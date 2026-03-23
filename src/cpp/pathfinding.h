// Pathfinding — Dijkstra first, then evolve to A*
#ifndef PATHFINDING_H
#define PATHFINDING_H

#include <vector>
#include <utility>

struct Point {
    int x;
    int y;
};

// Represents a node in the priority queue
struct Node {
    Point position;
    int cost; // cost_so_far (Dijkstra) — later: cost_so_far + heuristic (A*)
};

// Comparison for priority queue (min-heap: lowest cost first)
struct CompareNode {
    bool operator()(const Node& a, const Node& b);
};

class Pathfinder {
public:
    // Initialize with grid dimensions
    Pathfinder(int width, int height);

    // Mark a cell as an obstacle (not traversable)
    void setObstacle(int x, int y);

    // Clear an obstacle from a cell
    void clearObstacle(int x, int y);

    // Check if a cell is walkable (in bounds and not an obstacle)
    bool isWalkable(int x, int y) const;

    // Get valid neighbors of a cell (up, down, left, right)
    std::vector<Point> getNeighbors(const Point& p) const;

    // Find shortest path from start to goal
    // Returns vector of Points from start to goal, or empty if no path
    std::vector<Point> findPath(const Point& start, const Point& goal);

    // TODO: Add when evolving to A*
    // Heuristic function — Manhattan distance from current to goal
    // int heuristic(const Point& a, const Point& b);

private:
    int width;
    int height;
    std::vector<std::vector<bool>> grid; // true = obstacle
};

#endif
