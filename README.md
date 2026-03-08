# Bug-Killing-Robot

An autonomous floor-based robot that uses computer vision to detect and kill bugs (insects) in a room.

## Goal

Build a robot that can autonomously patrol a room, identify bugs using a trained object detection model, navigate toward them, and eliminate them — no human intervention required.

## Tech Stack

| Component | Technology |
|---|---|
| Vision Model | YOLOv8 (ultralytics) |
| Computer Vision | OpenCV |
| Language (Compute) | Python |
| Language (Firmware) | C++ |
| Serial Comms | pyserial |

## Dependencies

**Python**
- `ultralytics` — YOLOv8 inference and training
- `opencv-python` — Camera capture and image processing
- `pyserial` — Serial communication with microcontroller
- `pyyaml` — Config file parsing
- `numpy` — Array operations

## MVP

The MVP is a floor-only robot with four core capabilities:

1. **Patrol** — Robot roams the room using a boustrophedon (lawnmower) pattern searching for bugs
2. **Detect** — Camera feed is processed through a YOLOv8 model to identify bugs and their position relative to the robot
3. **Chase** — A* pathfinding to navigate toward the detected bug
4. **Kill** — Once close enough, activate kill mechanism

**State Machine:**
```
PATROL → bug detected → CHASE → close enough → KILL → bug gone → PATROL
  ↓                       ↓
obstacle               obstacle
  ↓                       ↓
AVOID → clear → PATROL  AVOID → clear → CHASE
```

## Future Improvements

- **Wall climbing** — Suction-based mechanism to drive on walls and ceilings
- **SLAM / mapping** — Build a map of the room for smarter navigation
- **Auto-charging** — Return to a charging dock when battery is low
- **Multi-bug tracking** — Track and prioritize multiple bugs simultaneously
- **Remote monitoring UI** — Web dashboard showing camera feed, robot status, and kill count
- **Surface detection** — Detect wall surface suitability before attempting to climb
- **IR camera** — Night vision for bug detection in low-light conditions
- **Bug species classification** — Identify bug type to adjust kill strategy

## File Structure

```
Bug-Killing-Robot/
├── README.md
├── requirements.txt
├── config.yaml                  # Robot settings (speeds, thresholds, serial port)
│
├── src/                         # Python - runs on Pi/Jetson
│   ├── main.py                  # Entry point, initializes and runs state machine
│   ├── state_machine.py         # PATROL / CHASE / KILL / AVOID states
│   ├── patrol.py                # Patrol patterns (random walk, sweep, etc.)
│   ├── vision.py                # YOLOv8 bug detection wrapper
│   ├── motor_controller.py      # Serial interface to Arduino
│   └── obstacle.py              # Ultrasonic sensor data handling
│
├── firmware/                    # C++ - runs on Arduino/ESP32
│   └── motor_driver/
│       └── motor_driver.ino     # Motor control + sensor reading
│
├── models/                      # Trained YOLO weights
│   └── .gitkeep
│
└── scripts/                     # Utilities
    └── train_model.py           # Fine-tune YOLOv8 on bug dataset
```
