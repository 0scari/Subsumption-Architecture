//
// Created by Oskars Pozdnakovs on 2019-02-11.
//

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
#ifndef P3_FUNCTIONS_H
#define P3_FUNCTIONS_H

void do_wheel_work(int wheel_id, string direction)
{
    unique_lock<std::mutex> lck(mtx, defer_lock);
    while (true) {
        lck.lock();
        cout << "Wheel " << wheel_id << " goes " << direction << endl;
        cond_var.wait(lck, [wheel_id] {
            if (!problem_spawner::problems.empty()) {
                problem_spawner::problem problem = problem_spawner::problems.front();
                return wheel_id == problem.wheel;
            }
            return false;

        });

        lck.unlock();
    }
};

void do_sensor_work(int sensor_id) {
    return;
    problem_spawner::problem problem = problem_spawner::problems.front();
//    switch (problem.type)
}





#endif //P3_FUNCTIONS_H

#pragma clang diagnostic pop