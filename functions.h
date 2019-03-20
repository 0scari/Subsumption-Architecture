//
// Created by Oskars Pozdnakovs on 2019-02-11.
//

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
#ifndef P3_FUNCTIONS_H
#define P3_FUNCTIONS_H

struct ResolutionInstrSet {
    string wheel1;
    string wheel2;
    string wheel3;
};

void do_wheel_work(Wheel* wheel)
{
    unique_lock<std::mutex> lck(wheel_mtx, defer_lock);
    while (true) {
        lck.lock();
        cout << "Wheel " << wheel->id << " goes " << wheel->direction << endl;
        wheel_cv.wait(lck, [wheel] {
            if (wheel->is_coordinator_signal_received) {
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
    if (severity > 6) {
        ResolutionInstrSet instructSet;
        instructSet.wheel1 = "to perform rock evasion maneuver\n";
        instructSet.wheel2 = "to perform rock evasion maneuver\n";
        instructSet.wheel3 = "to perform rock evasion maneuver\n";
    } else {
        ResolutionInstrSet instructSet;
        instructSet.wheel1 = "forward\n";
        instructSet.wheel2 = "forward\n";
        instructSet.wheel3 = "forward\n";
    }
}

void resolve_sand_problem(int severity) {
    if (severity < 4) {
        ResolutionInstrSet instructSet;
        instructSet.wheel1 = "forward\n";
        instructSet.wheel2 = "forward\n";
        instructSet.wheel3 = "forward\n";
    } else if (severity < 6) {
        ResolutionInstrSet instructSet;
        instructSet.wheel1 = "full throttle forward\n";
        instructSet.wheel2 = "full throttle forward\n";
        instructSet.wheel3 = "full throttle forward\n";
    } else {
        cout << "Requesting assistance from the Earth\n";
        ResolutionInstrSet instructSet;
        instructSet.wheel1 = "nowhere\n";
        instructSet.wheel2 = "nowhere\n";
        instructSet.wheel3 = "nowhere\n";
    }
}

void resolve_cliff_edge_problem(int severity) {
    if (severity < 2) {
        ResolutionInstrSet instructSet;
        instructSet.wheel1 = "forward\n";
        instructSet.wheel2 = "forward\n";
        instructSet.wheel3 = "forward\n";
    } else {
        cout << "Requesting assistance from the Earth\n";
        ResolutionInstrSet instructSet;
        instructSet.wheel1 = "nowhere\n";
        instructSet.wheel2 = "nowhere\n";
        instructSet.wheel3 = "nowhere\n";
    }
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
//            thread t(resolve_rock_problem, problem.severity);
            resolver_threads.emplace_back(resolve_rock_problem, problem.severity);
        } else if (problem.type == "sand") {
//            thread t(resolve_sand_problem, problem.severity);
            resolver_threads.emplace_back(resolve_sand_problem, problem.severity);
        } else if (problem.type == "cliff_edge") {
//            thread t(resolve_cliff_edge_problem, problem.severity);
            resolver_threads.emplace_back(resolve_cliff_edge_problem, problem.severity);
        };
        problem_spawner::problems.erase(problem_spawner::problems.begin());
    }
}

#endif //P3_FUNCTIONS_H

#pragma clang diagnostic pop