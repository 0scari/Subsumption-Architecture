//
// Created by Oskars Pozdnakovs on 2019-02-11.
//

#ifndef P3_FUNCTIONS_H
#define P3_FUNCTIONS_H

void do_wheel_work(int wheel_id, string direction)
{
    unique_lock<std::mutex> lck(mtx);
//    lck.lock();
//    cond_var.wait(lck);
    cout << "Wheel " << wheel_id << " goes " << direction << endl;
    lck.unlock();
//    cond_var.notify_all();
};





#endif //P3_FUNCTIONS_H
