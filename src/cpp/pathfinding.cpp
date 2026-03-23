// Pathfinding — Dijkstra first, then evolve to A*
#include "pathfinding.h"
#include <queue>
#include <unordered_map>
#include <algorithm>

bool CompareNode::operator()(const Node& a, const Node& b) {
    // TODO: implement — which node should have lower priority?
}

Pathfinder::Pathfinder(int width, int height) {
    // TODO: initialize grid
}

void Pathfinder::setObstacle(int x, int y) {
    // TODO: mark cell as obstacle
}

void Pathfinder::clearObstacle(int x, int y) {
    // TODO: clear obstacle from cell
}

bool Pathfinder::isWalkable(int x, int y) const {
    // TODO: check bounds and obstacle status
}

std::vector<Point> Pathfinder::getNeighbors(const Point& p) const {
    // TODO: return walkable neighbors (4-directional: up, down, left, right)
}

std::vector<Point> Pathfinder::findPath(const Point& start, const Point& goal) {
    // TODO: Dijkstra's algorithm
    //
    // Steps:
    // 1. Create a priority queue, push start with cost 0
    // 2. Track cost_so_far for each visited cell
    // 3. Track came_from to reconstruct the path
    // 4. While queue is not empty:
    //    a. Pop lowest cost node
    //    b. If it's the goal, reconstruct and return path
    //    c. For each neighbor:
    //       - Calculate new_cost = cost_so_far[current] + 1
    //       - If neighbor not visited or new_cost < cost_so_far[neighbor]:
    //         Update cost, record came_from, push to queue
    // 5. Return empty vector if no path found
    //
    // When evolving to A*:
    //   Change priority from cost_so_far to cost_so_far + heuristic(neighbor, goal)
}
