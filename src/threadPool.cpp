#include "threadPool.hpp"

void threadPool::initThreadpool() {
    for (int i = 0; i < this->threadList.size(); i++) {
        auto workTask = [this, i]() {
            while (runningStatus) {
                bool isSuccess = false;
                taskType workFunc;
                if (taskQueue.empty()) {
                    std::unique_lock<std::mutex> lockerInitFunc(poolLock);
                    cV.wait(lockerInitFunc);
                }
                else {
                    isSuccess = taskQueue.pop(workFunc);
                }
                if (isSuccess) {
                    workFunc();
                }
            }
        };
        threadList[i] = std::thread(workTask);
    }
}

threadPool::threadPool(int threadNum = 5) {
    this->runningStatus = true; this->threadList.resize(threadNum);
    // this->printMutex("[ThreadPool]: ThreadPool init!\n");
    this->initThreadpool();
}


threadPool::~threadPool() {
    this->runningStatus = false;
    this->cV.notify_all();
    printMutex("[ThreadPool]: Every threads joining!\n");
    for (auto& thread_ : this->threadList) {
        if (thread_.joinable()) {
            thread_.join();
        }
    }
    printMutex("[ThreadPool]: ThreadPool shut down!\n");
}