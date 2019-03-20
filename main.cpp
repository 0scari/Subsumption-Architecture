#include <iostream>
#include <string>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <cstdlib>
#include <vector>

using namespace std;

std::mutex mtx;
std::condition_variable cond_var;

#include "problem-spawner.h"
#include "functions.h"

int main(void)
{
    thread wheel1(do_wheel_work, 1, "forward");
    thread wheel2(do_wheel_work, 2, "forward");
    thread wheel3(do_wheel_work, 3, "forward");

    while (true) {
        thread spawner(problem_spawner::spawn_random_each_sec);
        spawner.join();
    }

    wheel1.join();
}