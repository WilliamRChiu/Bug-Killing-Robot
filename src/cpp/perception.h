#ifndef PERCEPTION_H
#define PERCEPTION_H

#include "pathfinding.h"  // Point

// What the perception layer reports for a single tick: sensor edges plus the
// positions they refer to. Booleans are edges (something just changed), not
// steady state.
struct Detection {
    bool bugAppeared      = false;
    bool bugLost          = false;
    bool bugInRange       = false;
    bool bugEliminated    = false;
    bool obstacleAppeared = false;
    bool obstacleCleared  = false;
    Point bugPosition{0, 0};
    Point obstaclePosition{0, 0};
};

// Seam between the mediator and the sensing hardware. Swap in a camera/ultrasonic
// implementation later; ManualPerception stands in while there is no hardware.
class Perception {
public:
    virtual ~Perception() = default;
    virtual Detection sense() = 0;
};

// Inputs are staged by hand (e.g. the REPL) and consumed exactly once per tick.
class ManualPerception final : public Perception {
public:
    void stage(const Detection& d) { next_ = d; }
    Detection sense() override {
        Detection d = next_;
        next_ = Detection{};
        return d;
    }

private:
    Detection next_{};
};

#endif  // PERCEPTION_H
