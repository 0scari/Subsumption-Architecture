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
            if (wheel->is_coordinator_signal_received) {
                wheel->is_coordinator_signal_received = false;
                return true;
            } else if (!problem_spawner::problems.empty()) {
                problem_spawner::problem problem = problem_spawner::problems.front();
                if (wheel->id == problem.wheel) {
                    wheel->sensor->is_activated = true;
                    sensor_cv.notify_all();
                }
            }
            return false;
        });
        lck.unlock();
    }
};

void resolve_rock_problem(int severity) {
    ResolutionInstrSet instruction_set;
    if (severity > 6) {
        instruction_set.wheel1 = "to perform rock evasion maneuver";
        instruction_set.wheel2 = "to perform rock evasion maneuver";
        instruction_set.wheel3 = "to perform rock evasion maneuver";
    } else {
        instruction_set.wheel1 = "forward";
        instruction_set.wheel2 = "forward";
        instruction_set.wheel3 = "forward";
    }
    coordinator.instuction_queue.push_back(instruction_set);
    coordinator_cv.notify_one();
}

void resolve_sand_problem(int severity) {
    ResolutionInstrSet instruction_set;
    if (severity < 4) {
        instruction_set.wheel1 = "forward";
        instruction_set.wheel2 = "forward";
        instruction_set.wheel3 = "forward";
    } else if (severity < 6) {
        instruction_set.wheel1 = "full throttle forward";
        instruction_set.wheel2 = "full throttle forward";
        instruction_set.wheel3 = "full throttle forward";
    } else {
        cout << "Requesting assistance from the Earth\n";
        instruction_set.wheel1 = "nowhere";
        instruction_set.wheel2 = "nowhere";
        instruction_set.wheel3 = "nowhere";
    }
    coordinator.instuction_queue.push_back(instruction_set);
    coordinator_cv.notify_one();
}

void resolve_cliff_edge_problem(int severity) {
    ResolutionInstrSet instruction_set;
    if (severity < 2) {
        instruction_set.wheel1 = "forward";
        instruction_set.wheel2 = "forward";
        instruction_set.wheel3 = "forward";
    } else {
        cout << "Requesting assistance from the Earth\n";
        instruction_set.wheel1 = "nowhere";
        instruction_set.wheel2 = "nowhere";
        instruction_set.wheel3 = "nowhere";
    }
    coordinator.instuction_queue.push_back(instruction_set);
    coordinator_cv.notify_one();
}

void do_sensor_work(Sensor* sensor) {
    while (true) {
        unique_lock<std::mutex> lck(sensor_mtx);
        sensor_cv.wait(lck, [sensor]{
            return sensor->is_activated;
        });
        sensor->is_activated = false;
        problem_spawner::problem problem = problem_spawner::problems.front();
        if (problem.type == "rock") {
            resolver_threads.emplace_back(resolve_rock_problem, problem.severity);
        } else if (problem.type == "sand") {
            resolver_threads.emplace_back(resolve_sand_problem, problem.severity);
        } else if (problem.type == "cliff_edge") {
            resolver_threads.emplace_back(resolve_cliff_edge_problem, problem.severity);
        };
        problem_spawner::problems.erase(problem_spawner::problems.begin());
    }
}

void do_coordinator_work() {
    while (true) {
        unique_lock<std::mutex> lck(manager_mtx);
        coordinator_cv.wait(lck, []{
            return !coordinator.instuction_queue.empty();
        });

        coordinator.wheels[0]->direction = coordinator.instuction_queue[0].wheel1;
        coordinator.wheels[0]->is_coordinator_signal_received = true;
        coordinator.wheels[1]->direction = coordinator.instuction_queue[0].wheel2;
        coordinator.wheels[1]->is_coordinator_signal_received = true;
        coordinator.wheels[2]->direction = coordinator.instuction_queue[0].wheel3;
        coordinator.wheels[2]->is_coordinator_signal_received = true;

        wheel_cv.notify_all();
        coordinator.instuction_queue.erase(coordinator.instuction_queue.begin());
        lck.unlock();
    }
}

#endif //P3_FUNCTIONS_H

#pragma clang diagnostic pop