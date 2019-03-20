//
// Created by Oskars Pozdnakovs on 2019-02-11.
//

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
#ifndef P3_FUNCTIONS_H
#define P3_FUNCTIONS_H

void do_wheel_work(Wheel* wheel)
{
    unique_lock<std::mutex> lck(wheel_mtx, defer_lock);
    while (true) {
        lck.lock();
        cout << "Wheel " << wheel->id << " goes " << wheel->direction << endl;
        wheel_cv.wait(lck, [wheel] {
            if (!problem_spawner::problems.empty()) {
                problem_spawner::problem problem = problem_spawner::problems.front();
                return wheel->id == problem.wheel;
            }
            return false;

        });
        lck.unlock();
    }
};

void do_sensor_work(int sensor_id) {
    unique_lock<std::mutex> lck(sensor_mtx);
    sensor_cv.wait(lck, []{ return false; });


    return;
    problem_spawner::problem problem = problem_spawner::problems.front();
//    switch (problem.type)
}





#endif //P3_FUNCTIONS_H

#pragma clang diagnostic pop