# Robot State Machine — Design (State Pattern, C++)

**Date:** 2026-07-14
**Location:** `src/cpp/state_machine.{h,cpp}`, plus `src/cpp/main.cpp` (manual driver)

## Goal

Implement the Bug-Killing-Robot control loop as a classic **State design pattern**:
a `State` base class with four concrete, `final` subclasses. Each state performs a
per-tick action and decides which state comes next. Because no hardware is connected
yet, "sensor" readings are supplied as a plain struct that can be set manually.

## States

Four states, matching the README state diagram:

- **PatrolState** — roam the room (boustrophedon sweep) searching for bugs.
- **ChaseState** — navigate toward a detected bug (pathfinding).
- **KillState** — activate the kill mechanism.
- **AvoidState** — steer around an obstacle, then return to whatever state was interrupted.

## Core interface

```cpp
// The "sensor snapshot". Set manually while there is no hardware.
struct Inputs {
    bool bugDetected   = false;
    bool obstacleAhead = false;
    bool bugInRange    = false;  // close enough to kill
    bool bugEliminated = false;  // kill confirmed
};

enum class StateId { Patrol, Chase, Kill, Avoid };

class State {
public:
    virtual ~State() = default;

    // Inspect inputs, perform this tick's action, and decide the next state.
    // Return nullptr to STAY in the current state (no reallocation per tick).
    virtual std::unique_ptr<State> update(const Inputs& in) = 0;

    virtual StateId id() const = 0;
    virtual const char* name() const = 0;
};
```

**Design choices**

- **Sensor-driven `update()`**: transitions are decided by inspecting `Inputs`, not by
  externally dispatched named events. The caller just supplies a fresh `Inputs` each tick.
- **`unique_ptr` instances**: each transition constructs a fresh state via
  `std::make_unique`. This lets `AvoidState` carry per-visit data (its return target).
- **`nullptr` means "stay"**: `update()` returns a new state only on an actual
  transition; returning `nullptr` keeps the current state and avoids per-tick allocation.

## Context

```cpp
class StateMachine {
    std::unique_ptr<State> current_;
public:
    StateMachine();                  // starts in PatrolState
    void tick(const Inputs& in);     // next = current_->update(in); if (next) swap
    const char* currentState() const;
    StateId currentId() const;
};
```

`tick()`:

```cpp
void StateMachine::tick(const Inputs& in) {
    if (auto next = current_->update(in)) {
        current_ = std::move(next);
    }
}
```

## Transition / action table

Every action a state can take and where it leads. First matching condition wins
(evaluated top to bottom within each state).

| State           | Condition (`Inputs`)         | Action                       | Next state              |
|-----------------|------------------------------|------------------------------|-------------------------|
| **PatrolState** | `obstacleAhead`              | —                            | Avoid (returnTo=Patrol) |
|                 | `bugDetected`                | —                            | Chase                   |
|                 | otherwise                    | drive boustrophedon sweep    | *stay*                  |
| **ChaseState**  | `obstacleAhead`              | —                            | Avoid (returnTo=Chase)  |
|                 | `bugInRange`                 | —                            | Kill                    |
|                 | `bugDetected`                | step along path toward bug   | *stay*                  |
|                 | `!bugDetected` (lost it)     | —                            | Patrol                  |
| **KillState**   | `bugEliminated`              | —                            | Patrol                  |
|                 | otherwise                    | activate kill mechanism      | *stay*                  |
| **AvoidState**  | `obstacleAhead` still true   | perform avoid maneuver       | *stay*                  |
|                 | `!obstacleAhead` (clear)     | —                            | returnTo (Patrol/Chase) |

## AvoidState return target

`AvoidState` is constructed with the `StateId` of the state it interrupted. When the
obstacle clears, it constructs a fresh instance of that state.

```cpp
class AvoidState final : public State {
    StateId returnTo_;  // Patrol or Chase
public:
    explicit AvoidState(StateId returnTo) : returnTo_(returnTo) {}
    std::unique_ptr<State> update(const Inputs& in) override;
    StateId id() const override { return StateId::Avoid; }
    const char* name() const override { return "AVOID"; }
};
```

On clear, `returnTo_` maps back to a concrete state: `Patrol -> make_unique<PatrolState>()`,
`Chase -> make_unique<ChaseState>()`.

## Kill mechanism (for now)

`KillState`'s action is a **stub/log only** — e.g. `log("KILL: activating mechanism")`.
No coupling to `motor_controller` yet (that interface is itself still a stub). This keeps
the state machine self-contained and testable.

## Manual driver (circumventing sensors)

`src/cpp/main.cpp` — an **interactive REPL** so the machine can be exercised today with no
hardware:

- Reads single keystrokes that toggle `Inputs` flags before each `tick()`:
  - `b` = bugDetected, `o` = obstacleAhead, `r` = bugInRange, `k` = bugEliminated
  - Enter / empty = tick with all flags false (or with the current toggles)
  - `q` = quit
- Prints `currentState()` after each tick.

Example session:

```
state=PATROL
> o          # obstacle ahead
state=AVOID
> (enter)    # clear
state=PATROL
> b          # bug detected
state=CHASE
> r          # in range
state=KILL
> k          # eliminated
state=PATROL
```

Exact flag semantics (momentary pulse vs. sticky toggle) to be pinned down in the
implementation plan; default assumption: each key sets its flag true for the next single
tick, then it resets.

## Files

- `src/cpp/state_machine.h` — `Inputs`, `StateId`, `State`, `StateMachine`, and the four
  concrete states. Kept together: they are small and tightly coupled.
- `src/cpp/state_machine.cpp` — `update()` implementations and `StateMachine`.
- `src/cpp/main.cpp` — interactive REPL driver.
- `src/cpp/CMakeLists.txt` — add the state-machine sources and the `main` target.

## Out of scope (YAGNI for now)

- Real sensor / camera integration.
- `motor_controller` coupling.
- `onEnter`/`onExit` hooks (actions live inside `update()`).
- Carrying bug position/coordinates through Chase (re-acquire on each visit for now).
```

