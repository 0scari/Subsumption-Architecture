#include <iostream>
#include <thread>
#include <condition_variable>
#include <vector>

using namespace std;

std::mutex wheel_mtx;
std::mutex sensor_mtx;

std::condition_variable wheel_cv;
std::condition_variable sensor_cv;

struct Sensor {
    int id;
    bool is_activated = false;
    void handle_rock_problem();
    void handle_sand_problem();
    void handle_cliff_edge_problem();
} sensor1, sensor2, sensor3;
void Sensor::handle_rock_problem() {
    cout << "rock\n";
}
void Sensor::handle_sand_problem() {
    cout << "sand\n";

}
void Sensor::handle_cliff_edge_problem() {
    cout << "cliff_edge\n";
}

struct Wheel {
    int id;
    string direction = "forward";
    Sensor* sensor;
} wheel1, wheel2, wheel3;

#include "problem-spawner.h"
#include "functions.h"



int main(void)
{
    wheel1.id = 1; sensor1.id = 1; wheel1.sensor = &sensor1;
    wheel2.id = 2; sensor2.id = 2; wheel2.sensor = &sensor2;
    wheel3.id = 3; sensor3.id = 3; wheel3.sensor = &sensor3;

    thread sensor_t_1(do_sensor_work, &sensor1);
    thread sensor_t_2(do_sensor_work, &sensor2);
    thread sensor_t_3(do_sensor_work, &sensor3);

    thread thread1(do_wheel_work, &wheel1);
    thread thread2(do_wheel_work, &wheel2);
    thread thread3(do_wheel_work, &wheel3);

    while (true) {
        thread spawner(problem_spawner::spawn_random_each_sec);
        spawner.join();
    }

    thread1.join();
}