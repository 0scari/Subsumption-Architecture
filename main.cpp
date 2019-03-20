#include <iostream>
#include <thread>
#include <condition_variable>
#include <vector>

using namespace std;

std::mutex mtx;
std::condition_variable cond_var;

#include "problem-spawner.h"
#include "functions.h"

int main(void)
{

    thread sensor1(do_sensor_work, 1);
    thread sensor2(do_sensor_work, 2);
    thread sensor3(do_sensor_work, 3);


    thread wheel1(do_wheel_work, 1, "forward");
    thread wheel2(do_wheel_work, 2, "forward");
    thread wheel3(do_wheel_work, 3, "forward");

    while (true) {
        thread spawner(problem_spawner::spawn_random_each_sec);
        spawner.join();
    }

    wheel1.join();
}