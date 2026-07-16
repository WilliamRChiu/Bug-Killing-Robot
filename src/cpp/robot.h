#ifndef ROBOT_H
#define ROBOT_H

#include <memory>
#include <string_view>

#include "pathfinding.h"
#include "perception.h"
#include "state_machine.h"
#include "world_model.h"

// Mediator. Owns the colleagues (perception, world model, pathfinder, current
// state) and coordinates them in tick(): sense -> update model -> drive the
// state machine -> let the current state act. The colleagues never reference
// each other directly; they go through the Robot.
class Robot {
public:
    explicit Robot(Perception& perception);

    void tick();

    void setState(std::unique_ptr<State> next) { state_ = std::move(next); }

    // Colleague access for states — they reach shared services via the mediator.
    WorldModel& world() { return world_; }
    const WorldModel& world() const { return world_; }
    Pathfinder& pathfinder() { return pathfinder_; }

    std::string_view getStateName() const { return state_->getStateName(); }

private:
    // Only tick() dispatches sensor edges into the state machine.
    void onBugDetected()      { state_->onBugDetected(); }
    void onBugLost()          { state_->onBugLost(); }
    void onObstacleDetected() { state_->onObstacleDetected(); }
    void onObstacleCleared()  { state_->onObstacleCleared(); }
    void onBugInRange()       { state_->onBugInRange(); }
    void onBugEliminated()    { state_->onBugEliminated(); }

    Perception& perception_;
    WorldModel world_;
    Pathfinder pathfinder_;
    std::unique_ptr<State> state_;
};

#endif  // ROBOT_H
