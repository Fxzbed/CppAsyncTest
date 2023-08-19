#ifndef _SAFEQUEUE_HPP_
#define _SAFEQUEUE_HPP_

#include <iostream>
#include <mutex>
#include <atomic>
#include <queue>
#include <thread>
#include <future>
#include <vector>
#include <functional>
#include <condition_variable>

template <typename T>
class safeQueue {
    private:
        std::mutex lockQ;
        std::queue<T> queueM;

    public:

        bool empty() {
            std::unique_lock<std::mutex> locker(lockQ);
            return queueM.empty();
        }

        int size() {
            std::unique_lock<std::mutex> locker(lockQ);
            return queueM.size();
        }

        void push(T& t) {
            std::unique_lock<std::mutex> locker(lockQ);
            queueM.emplace(t);
            return;
        }

        void push(T&& t) {
            std::unique_lock<std::mutex> locker(lockQ);
            queueM.emplace(std::move(t));
        }

        bool pop(T& t) {
            std::unique_lock<std::mutex> locker(lockQ);
            if (queueM.empty()) {
                return false;
            }
            else {
                t = std::move(queueM.front());
                queueM.pop();
                return true;
            }
            return true;
        }
};
// self mutex queue class 

#endif _SAFEQUEUE_HPP_