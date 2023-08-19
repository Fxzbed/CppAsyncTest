#pragma once

#include "safeQueue.hpp"

struct outPut {
    std::mutex lockPrint;
    void operator()(const char* str) {
        std::unique_lock<std::mutex> locker(lockPrint);
        std::cout << str;
    }
};

#define Debug() {std::cout << "Debug:" << "Error happened! error in line of " << __LINE__ << '\n';}

using taskType = std::function<void()>;

class threadPool
{
    public:
        outPut printMutex;
        threadPool(const threadPool&) = delete;
        threadPool& operator = (const threadPool&) = delete;
        threadPool(const threadPool&&) = delete;
        threadPool& operator = (const threadPool&&) = delete;


        threadPool(int threadNum = 5);
        ~threadPool();

        template<typename Func, typename ...Args>
        auto submitTask(Func&& funcReturn, Args... argSend) -> std::future<decltype(funcReturn(argSend...))>{
            using returnType = typename std::invoke_result<Func, Args...>::type;
            std::function<returnType()> transF1 = std::bind(std::forward<Func>(funcReturn), std::forward<Args>(argSend)...);
            auto transF2 = std::make_shared<std::packaged_task<returnType()>>(transF1);
            
            taskType transF3 = [transF2]() {
                (*transF2)();
            };
            this->taskQueue.push(transF3);
            this->cV.notify_one();

            return transF2->get_future();
        }
        

    private:
        std::mutex poolLock;                    // Lock for all threadpool
        std::atomic<bool> runningStatus;        // atomic for recording threadpool status
        safeQueue<taskType> taskQueue;         // save tasks to submit to threads
        std::vector<std::thread> threadList;    // save threads
        std::condition_variable cV;             // Block threads

        void initThreadpool();
};