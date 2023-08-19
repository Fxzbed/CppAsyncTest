#include "webServer.hpp"
#include "identify.hpp"
#include "threadPool.hpp"

void project_run_web() {
    webServer Server;   //8888
    Server.serverInit();    
}

void project_run_identify() {
    ServerSocket_identify Server;   //8080
    Server.socketServerRun();
}

int main() {
    threadPool pool(3);
    pool.submitTask(project_run_web);
    pool.submitTask(project_run_identify);
    spdlog::info("Task submit in threadpool");
    while (1) { }
}