//
// Created by Trebing, Peter on 2019-08-27.
//

#ifndef COLLISIONSIM_SYNCHRONIZEDLIST_H
#define COLLISIONSIM_SYNCHRONIZEDLIST_H

#include <future>
#include <thread>
#include <vector>

template<typename T>
class SynchronizedList {

public:

    SynchronizedList() {}

    ~SynchronizedList() {}

    std::size_t size() {
        std::lock_guard<std::recursive_mutex> uLock(_mutex);
        return _items.size();
    }

    bool empty() {
        std::lock_guard<std::recursive_mutex> myLock(_mutex);
        return _items.empty();
    }

    std::shared_ptr<T> popBack() {
        // perform vector modification under the lock
        std::lock_guard<std::recursive_mutex> uLock(_mutex);
        // remove last vector keys from queue
        std::shared_ptr<T> v = std::move(_items.back());
        _items.pop_back();
        return v; // will not be copied due to return value optimization (RVO) in C++
    }

    void pushBack(std::shared_ptr<T> &&v) {
        // perform vector modification under the lock
        std::lock_guard<std::recursive_mutex> uLock(_mutex);
        _items.emplace_back(std::move(v));

    }

    void erase(std::size_t pos) {
        // perform vector modification under the lock
        std::lock_guard<std::recursive_mutex> uLock(_mutex);
        if (pos < _items.size() - 1) {
            _items.erase(_items.begin() + pos);
        }
    }

    void map(const std::function<bool(std::shared_ptr<T> &part, size_t)> &f) {
        // perform lambda operation under the lock
        std::lock_guard<std::recursive_mutex> uLock(_mutex);
        size_t i = 0;
        for (auto &part: _items) {
            if (f(part, i++)) break;
        }
    }

private:
    std::vector<std::shared_ptr<T>> _items; // list of all items in the simulationn
    std::recursive_mutex _mutex;

};

#endif //COLLISIONSIM_SYNCHRONIZEDLIST_H
