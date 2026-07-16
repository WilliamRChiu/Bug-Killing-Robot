#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

#include <string_view>

class Robot;

enum class StateId { Patrol, Chase };

class State {
public:
    virtual ~State() = default;

    virtual void onBugDetected() = 0;
    virtual void onBugLost() = 0;
    virtual void onObstacleDetected() = 0;
    virtual void onObstacleCleared() = 0;
    virtual void onBugInRange() = 0;
    virtual void onBugEliminated() = 0;
    virtual void run() = 0;

    virtual std::string_view getStateName() const = 0;

protected:
    State() = default;
};

class PatrolState final : public State {
public:
    explicit PatrolState(Robot& robot) : robot_(robot) {}
    void onBugDetected() override;
    void onBugLost() override {}
    void onObstacleDetected() override;
    void onObstacleCleared() override {}
    void onBugInRange() override {}
    void onBugEliminated() override {}
    void run() override;
    std::string_view getStateName() const override { return "PATROL"; }

private:
    Robot& robot_;
};

class ChaseState final : public State {
public:
    explicit ChaseState(Robot& robot) : robot_(robot) {}
    void onBugDetected() override {}
    void onBugLost() override;
    void onObstacleDetected() override;
    void onObstacleCleared() override {}
    void onBugInRange() override;
    void onBugEliminated() override {}
    void run() override;
    std::string_view getStateName() const override { return "CHASE"; }

private:
    Robot& robot_;
};

class KillState final : public State {
public:
    explicit KillState(Robot& robot) : robot_(robot) {}
    void onBugDetected() override {}
    void onBugLost() override {}
    void onObstacleDetected() override {}
    void onObstacleCleared() override {}
    void onBugInRange() override {}
    void onBugEliminated() override;
    void run() override;
    std::string_view getStateName() const override { return "KILL"; }

private:
    Robot& robot_;
};

class AvoidState final : public State {
public:
    AvoidState(Robot& robot, StateId returnTo) : robot_(robot), returnTo_(returnTo) {}
    void onBugDetected() override {}
    void onBugLost() override {}
    void onObstacleDetected() override {}
    void onObstacleCleared() override;
    void onBugInRange() override {}
    void onBugEliminated() override {}
    void run() override;
    std::string_view getStateName() const override { return "AVOID"; }

private:
    Robot& robot_;
    StateId returnTo_;
};

#endif  // STATE_MACHINE_H
