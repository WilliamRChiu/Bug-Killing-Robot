// Pathfinding — Dijkstra first, then evolve to A*
#include "pathfinding.h"
#include <queue>
#include <unordered_map>
#include <algorithm>
#include <stdexcept>
#include <string>

bool CompareNode::operator()(const Node& a, const Node& b) {
    // priority_queue is a max-heap by default; invert so the
    // lowest-cost node has the highest priority (min-heap behavior).
    return a.cost > b.cost;
}

namespace {
// Format a coordinate for error messages, e.g. "(3, 5)".
std::string coordStr(int x, int y) {
    return "(" + std::to_string(x) + ", " + std::to_string(y) + ")";
}
}

Pathfinder::Pathfinder(int width, int height)
    : width(width), height(height),
      grid() {
    if (width <= 0 || height <= 0) {
        throw std::invalid_argument(
            "Pathfinder: dimensions must be positive, got " +
            coordStr(width, height));
    }
    grid.assign(width, std::vector<bool>(height, false));
}

void Pathfinder::setObstacle(int x, int y) {
    if (x < 0 || x >= width || y < 0 || y >= height) {
        throw std::out_of_range("setObstacle: coordinate out of bounds " +
                                coordStr(x, y));
    }
    grid[x][y] = true;
}

void Pathfinder::clearObstacle(int x, int y) {
    if (x < 0 || x >= width || y < 0 || y >= height) {
        throw std::out_of_range("clearObstacle: coordinate out of bounds " +
                                coordStr(x, y));
    }
    grid[x][y] = false;
}

bool Pathfinder::isWalkable(int x, int y) const {
    return x >= 0 && x < width && y >= 0 && y < height && !grid[x][y];
}

std::vector<Point> Pathfinder::getNeighbors(const Point& p) const {
    std::vector<Point> neighbors;
    const int dx[] = {0, 0, -1, 1}; // up, down, left, right
    const int dy[] = {-1, 1, 0, 0};
    for (int i = 0; i < 4; ++i) {
        int nx = p.x + dx[i];
        int ny = p.y + dy[i];
        if (isWalkable(nx, ny)) {
            neighbors.push_back(Point{nx, ny});
        }
    }
    return neighbors;
}

std::vector<Point> Pathfinder::findPath(const Point& start, const Point& goal) {
    // Validate endpoints. An empty result from this point on means strictly
    // "no path found" — never "bad input".
    auto validate = [this](const Point& p, const char* label) {
        if (p.x < 0 || p.x >= width || p.y < 0 || p.y >= height) {
            throw std::invalid_argument(std::string("findPath: ") + label +
                                        " out of bounds " + coordStr(p.x, p.y));
        }
        if (grid[p.x][p.y]) {
            throw std::invalid_argument(std::string("findPath: ") + label +
                                        " is on an obstacle " +
                                        coordStr(p.x, p.y));
        }
    };
    validate(start, "start");
    validate(goal, "goal");

    // Encode a Point as a single int key so it can index the maps below.
    auto key = [this](const Point& p) { return p.y * width + p.x; };

    std::priority_queue<Node, std::vector<Node>, CompareNode> frontier;
    std::unordered_map<int, int> cost_so_far;
    std::unordered_map<int, Point> came_from;

    frontier.push(Node{start, 0});
    cost_so_far[key(start)] = 0;

    while (!frontier.empty()) {
        Node current = frontier.top();
        frontier.pop();

        if (current.position.x == goal.x && current.position.y == goal.y) {
            // Reconstruct path from goal back to start, then reverse.
            std::vector<Point> path;
            Point step = goal;
            while (!(step.x == start.x && step.y == start.y)) {
                path.push_back(step);
                step = came_from[key(step)];
            }
            path.push_back(start);
            std::reverse(path.begin(), path.end());
            return path;
        }

        // Skip stale queue entries superseded by a cheaper path.
        if (current.cost > cost_so_far[key(current.position)]) {
            continue;
        }

        for (const Point& next : getNeighbors(current.position)) {
            int new_cost = cost_so_far[key(current.position)] + 1;
            int next_key = key(next);
            auto it = cost_so_far.find(next_key);
            if (it == cost_so_far.end() || new_cost < it->second) {
                cost_so_far[next_key] = new_cost;
                came_from[next_key] = current.position;
                frontier.push(Node{next, new_cost});
            }
        }
    }

    return {}; // no path found
}
