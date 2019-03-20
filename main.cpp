#include <iostream>
#include <thread>
#include <condition_variable>
#include <vector>

using namespace std;

std::mutex wheel_mtx;
std::mutex sensor_mtx;

std::condition_variable wheel_cv;
std::condition_variable sensor_cv;

struct Wheel {
    int id;
    string direction = "forward";
} wheel1, wheel2, wheel3;

#include "problem-spawner.h"
#include "functions.h"



int main(void)
{
    wheel1.id = 1;
    wheel2.id = 2;
    wheel3.id = 3;
//    thread sensor1(do_sensor_work, 1);
//    thread sensor2(do_sensor_work, 2);
//    thread sensor3(do_sensor_work, 3);

    thread thread1(do_wheel_work, &wheel1);
    thread thread2(do_wheel_work, &wheel2);
    thread thread3(do_wheel_work, &wheel3);

    while (true) {
        thread spawner(problem_spawner::spawn_random_each_sec);
        spawner.join();
    }

    thread1.join();
}