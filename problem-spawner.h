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
    vector<problem> encountered_problems;

    void spawn_random_each_sec() {
        this_thread::sleep_for(chrono::milliseconds(2000));
        problem aProblem;
        aProblem.type = problem_types[(rand() % 2) + 0];
        aProblem.wheel = (rand() % 3) + 1;
        encountered_problems.push_back(aProblem);
    }
}

#endif //SUBSUMPTION_ARCHITECTURE_PROBLEM_SPAWNER_H
