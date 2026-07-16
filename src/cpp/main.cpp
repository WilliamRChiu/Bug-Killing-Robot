// Interactive REPL driver for the robot mediator.
//
// No hardware yet, so detections are staged by hand. Each line stages a
// Detection, then runs one Robot::tick():
//
//   b = bug appeared (at (5,5))   l = bug lost
//   o = obstacle appeared (2,2)   c = obstacle cleared
//   r = bug in range              k = bug eliminated
//   (empty line)                  = tick with no detections
//   q                             = quit
#include <iostream>
#include <string>

#include "perception.h"
#include "robot.h"

int main() {
    ManualPerception perception;
    Robot robot(perception);

    std::cout << "Robot mediator REPL. Detections: b=bug l=bugLost o=obstacle "
                 "c=cleared r=inRange k=eliminated, q=quit.\n";
    std::cout << "state=" << robot.getStateName() << "\n";

    std::string line;
    while (true) {
        std::cout << "> ";
        if (!std::getline(std::cin, line)) {
            break;  // EOF
        }

        Detection d;
        bool quit = false;
        for (char c : line) {
            switch (c) {
                case 'b': d.bugAppeared = true; d.bugPosition = Point{5, 5}; break;
                case 'l': d.bugLost = true; break;
                case 'o': d.obstacleAppeared = true; d.obstaclePosition = Point{2, 2}; break;
                case 'c': d.obstacleCleared = true; break;
                case 'r': d.bugInRange = true; break;
                case 'k': d.bugEliminated = true; break;
                case 'q': quit = true; break;
                default: break;  // ignore spaces/unknown chars
            }
        }
        if (quit) {
            break;
        }

        perception.stage(d);
        robot.tick();
        std::cout << "state=" << robot.getStateName() << "\n";
    }

    return 0;
}
