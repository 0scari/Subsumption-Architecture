#include <iostream>
#include <thread>
#include <condition_variable>
#include <vector>

using namespace std;

std::mutex wheel_mtx;
std::mutex sensor_mtx;
std::mutex manager_mtx;

std::condition_variable wheel_cv;
std::condition_variable sensor_cv;
std::condition_variable manager_cv;

vector <thread> resolver_threads;

struct Sensor {
    bool is_activated = false;
} sensor1, sensor2, sensor3;

struct Wheel {
    int id;
    static bool is_coordinator_signal_received;
    string direction = "forward";
    Sensor* sensor;
} wheel1, wheel2, wheel3;
bool Wheel::is_coordinator_signal_received = false;

struct ResolutionInstrSet {
    string wheel1;
    string wheel2;
    string wheel3;
};

struct Coordinator {
    Wheel wheels[3];
    vector<ResolutionInstrSet> instuction_queue;
} coordinator;

#include "problem-spawner.h"
#include "functions.h"



int main(void)
{
    wheel1.id = 1; wheel1.sensor = &sensor1;
    wheel2.id = 2; wheel2.sensor = &sensor2;
    wheel3.id = 3; wheel3.sensor = &sensor3;

    coordinator.wheels[0] = wheel1;
    coordinator.wheels[1] = wheel2;
    coordinator.wheels[2] = wheel3;

    thread coordinator_t(do_coordinator_work);

    thread sensor_t_1(do_sensor_work, &sensor1);
    thread sensor_t_2(do_sensor_work, &sensor2);
    thread sensor_t_3(do_sensor_work, &sensor3);

    thread wheel_t1(do_wheel_work, &wheel1);
    thread wheel_t2(do_wheel_work, &wheel2);
    thread wheel_t3(do_wheel_work, &wheel3);

    while (true) {
        thread spawner(problem_spawner::spawn_random_each_sec);
        spawner.join();
    }
}