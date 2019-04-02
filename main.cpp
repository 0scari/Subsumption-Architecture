#include <iostream>
#include <thread>
#include <condition_variable>
#include <vector>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"

using namespace std;

std::mutex wheel_mtx;
std::mutex sensor_mtx;
std::mutex coordinator_mtx;

std::condition_variable wheel_cv;
std::condition_variable sensor_cv;
std::condition_variable coordinator_cv;

/**
 * A list of threads where the resolver operations are executed
 */
vector <thread> resolver_threads;

#include "structs.h"
#include "problem-spawner.h"
#include "work_functions.h"

int main()
{
    wheel1.id = 1; wheel1.sensor = &sensor1;
    wheel2.id = 2; wheel2.sensor = &sensor2;
    wheel3.id = 3; wheel3.sensor = &sensor3;

    coordinator.wheels[0] = &wheel1;
    coordinator.wheels[1] = &wheel2;
    coordinator.wheels[2] = &wheel3;

    thread coordinator_t(do_coordinator_work);

    thread sensor_t1(do_sensor_work, &sensor1);
    thread sensor_t2(do_sensor_work, &sensor2);
    thread sensor_t3(do_sensor_work, &sensor3);

    thread wheel_t1(do_wheel_work, &wheel1);
    thread wheel_t2(do_wheel_work, &wheel2);
    thread wheel_t3(do_wheel_work, &wheel3);

// locking the app in an infinite loop
    while (true) {
        thread spawner(problem_spawner::respawn);
        spawner.join();
    }
}
#pragma clang diagnostic pop