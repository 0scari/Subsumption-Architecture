//
// Created by Oskars Pozdnakovs on 2019-03-20.
//

#ifndef SUBSUMPTION_ARCHITECTURE_PROBLEM_SPAWNER_H
#define SUBSUMPTION_ARCHITECTURE_PROBLEM_SPAWNER_H

namespace problem_spawner {

    struct Problem {
        string type;
        int severity;
        int wheel;
    };
    string problem_types[] = {"rock", "cliff_edge", "sand"};
    vector<Problem> problems;

    /**
     * Create a new_problem, append it to the problem list
     * and notify the wheels.
     */
    void respawn() {
        this_thread::sleep_for(chrono::milliseconds(500));

        Problem new_problem;
        new_problem.type = problem_types[(rand() % 3) + 0];
        new_problem.severity = (rand() % 11) + 1;
        new_problem.wheel = (rand() % 3) + 1;
        problems.push_back(new_problem);

        cout << "❗️New problem: " << new_problem.type
             << " for wheel: " << new_problem.wheel
             << " severity: " << new_problem.severity << "❗️\n";

        wheel_cv.notify_all();
    }
}

#endif //SUBSUMPTION_ARCHITECTURE_PROBLEM_SPAWNER_H
