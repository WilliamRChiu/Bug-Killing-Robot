#include "robot.h"

#include <stdexcept>

namespace {
constexpr int kGridWidth = 20;   // TODO: source grid size from config.yaml
constexpr int kGridHeight = 20;
}  // namespace

Robot::Robot(Perception& perception)
    : perception_(perception),
      pathfinder_(kGridWidth, kGridHeight),
      state_(std::make_unique<PatrolState>(*this)) {}

void Robot::tick() {
    Detection d = perception_.sense();
    world_.update(d);

    // Feed a detected obstacle into the shared map so pathfinding routes around
    // it. Out-of-range cells are ignored for now (scaffold).
    if (d.obstacleAppeared) {
        try {
            pathfinder_.setObstacle(d.obstaclePosition.x, d.obstaclePosition.y);
        } catch (const std::out_of_range&) {
        }
    }

    // Drive the state machine from the sensor edges.
    if (d.bugAppeared)      onBugDetected();
    if (d.bugLost)          onBugLost();
    if (d.obstacleAppeared) onObstacleDetected();
    if (d.obstacleCleared)  onObstacleCleared();
    if (d.bugInRange)       onBugInRange();
    if (d.bugEliminated)    onBugEliminated();

    // The resulting state acts, reading the world model / pathfinder as needed.
    state_->run();
}
