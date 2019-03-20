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
        wheel->sensor->is_activated = true;
        sensor_cv.notify_all();
        lck.unlock();
    }
};

void do_sensor_work(Sensor* sensor) {
    unique_lock<std::mutex> lck(sensor_mtx);
    sensor_cv.wait(lck, [sensor]{
        return sensor->is_activated;
    });
    problem_spawner::problem problem = problem_spawner::problems.front();
    if (problem.type == "rock") sensor->handle_rock_problem();
    else if (problem.type == "sand") sensor->handle_sand_problem();
    else if (problem.type == "cliff_edge") sensor->handle_cliff_edge_problem();
    problem_spawner::problems.erase(problem_spawner::problems.begin());
}





#endif //P3_FUNCTIONS_H

#pragma clang diagnostic pop