#include "state_machine.h"

#include <exception>
#include <iostream>
#include <memory>

#include "robot.h"

void PatrolState::onObstacleDetected() {
    robot_.setState(std::make_unique<AvoidState>(robot_, StateId::Patrol));
}

void PatrolState::onBugDetected() {
    robot_.setState(std::make_unique<ChaseState>(robot_));
}

void PatrolState::run() {
    std::cout << "PATROL: sweeping\n";
}

void ChaseState::onObstacleDetected() {
    robot_.setState(std::make_unique<AvoidState>(robot_, StateId::Chase));
}

void ChaseState::onBugInRange() {
    robot_.setState(std::make_unique<KillState>(robot_));
}

void ChaseState::onBugLost() {
    robot_.setState(std::make_unique<PatrolState>(robot_));
}

void ChaseState::run() {
    if (!robot_.world().hasBug()) {
        std::cout << "CHASE: advancing toward bug\n";
        return;
    }
    const Point bug = robot_.world().bugPosition();
    const Point me = robot_.world().robotPosition();
    try {
        auto path = robot_.pathfinder().findPath(me, bug);
        std::cout << "CHASE: planning toward bug at (" << bug.x << ", " << bug.y
                  << "), path length " << path.size() << "\n";
    } catch (const std::exception& e) {
        std::cout << "CHASE: cannot plan to bug at (" << bug.x << ", " << bug.y
                  << "): " << e.what() << "\n";
    }
}

void KillState::onBugEliminated() {
    robot_.setState(std::make_unique<PatrolState>(robot_));
}

void KillState::run() {
    std::cout << "KILL: activating mechanism\n";
}

void AvoidState::onObstacleCleared() {
    if (returnTo_ == StateId::Chase) {
        robot_.setState(std::make_unique<ChaseState>(robot_));
    } else {
        robot_.setState(std::make_unique<PatrolState>(robot_));
    }
}

void AvoidState::run() {
    std::cout << "AVOID: maneuvering around obstacle\n";
}
