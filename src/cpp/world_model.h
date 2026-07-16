#ifndef WORLD_MODEL_H
#define WORLD_MODEL_H

#include "pathfinding.h"  // Point
#include "perception.h"   // Detection

// Shared blackboard the mediator owns. Perception writes it; states read it.
// It carries positional data that the parameterless state events cannot.
class WorldModel {
public:
    void update(const Detection& d) {
        if (d.bugAppeared) {
            hasBug_ = true;
            bugPosition_ = d.bugPosition;
        }
        if (d.bugLost || d.bugEliminated) {
            hasBug_ = false;
        }
    }

    bool hasBug() const { return hasBug_; }
    Point bugPosition() const { return bugPosition_; }
    Point robotPosition() const { return robotPosition_; }

private:
    bool hasBug_ = false;
    Point bugPosition_{0, 0};
    Point robotPosition_{0, 0};  // TODO: update from odometry/localization
};

#endif  // WORLD_MODEL_H
