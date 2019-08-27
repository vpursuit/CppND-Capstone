//
// Created by Trebing, Peter on 2019-08-27.
//

#ifndef COLLISIONSIM_STOPPABLE_H
#define COLLISIONSIM_STOPPABLE_H

#include <iostream>
#include <assert.h>
#include <chrono>
#include <future>

/*
* Class that encapsulates promise and future object and
* provides API to set exit signal for the thread
*/
class Stoppable {

public:

    Stoppable() :
            futureObj(exitSignal.get_future()) {
    }

    Stoppable(Stoppable &&obj) : exitSignal(std::move(obj.exitSignal)), futureObj(std::move(obj.futureObj)) {
        std::cout << "Move Constructor is called" << std::endl;
    }

    Stoppable &operator=(Stoppable &&obj) {
        std::cout << "Move Assignment is called" << std::endl;
        exitSignal = std::move(obj.exitSignal);
        futureObj = std::move(obj.futureObj);
        return *this;
    }

    virtual void run() = 0;

    void operator()() {
        run();
    }

    bool stopRequested() {
        if (futureObj.wait_for(std::chrono::milliseconds(0)) == std::future_status::timeout) {
            return false;
        } else {
            return true;
        }
    }

    void stop() {
        exitSignal.set_value();
    }


    std::promise<void> exitSignal;
    std::future<void> futureObj;

};

#endif

