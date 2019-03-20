//
// Created by Oskars Pozdnakovs on 2019-03-20.
//

#ifndef SUBSUMPTION_ARCHITECTURE_PROBLEM_SPAWNER_H
#define SUBSUMPTION_ARCHITECTURE_PROBLEM_SPAWNER_H

namespace problem_spawner {
    string problem_types[] = {"rock", "cliff_edge", "sand"};

    struct problem {
        string type;
        int wheel;
    };
    vector<problem> problems;

    void spawn_random_each_sec() {
        this_thread::sleep_for(chrono::milliseconds(2000));
        problem new_problem;
        new_problem.type = problem_types[(rand() % 2) + 0];
        new_problem.wheel = (rand() % 3) + 1;
        problems.push_back(new_problem);
        cout << "New problem: " << new_problem.type << " for wheel: " << new_problem.wheel << endl;
        wheel_cv.notify_all();
    }
}

#endif //SUBSUMPTION_ARCHITECTURE_PROBLEM_SPAWNER_H
