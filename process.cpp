#include "process.h"
#include <iostream>
#include <chrono>
#include <thread>

Process::Process(int id, const std::vector<Resource*>& resources)
    : id(id), resources(resources) {}

void Process::run() {
    std::this_thread::sleep_for(std::chrono::milliseconds(100 * id));
    std::cout << "[Process " << id << "] started." << std::endl;

    // Acquire first resource
    std::cout << "[Process " << id << "] attempting to lock Resource " << resources[0]->id << std::endl;
    resources[0]->mtx.lock();
    resources[0]->inUse = true;
    std::cout << "[Process " << id << "] acquired Resource " << resources[0]->id << std::endl;

    std::this_thread::sleep_for(std::chrono::milliseconds(500)); // simulate work

    // Try to acquire second resource
    std::cout << "[Process " << id << "] attempting to lock Resource " << resources[1]->id << std::endl;

    waiting = true;

    while (!killed) {
        if (resources[1]->mtx.try_lock()) {
            waiting = false;
            resources[1]->inUse = true;
            std::cout << "[Process " << id << "] acquired Resource " << resources[1]->id << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(500)); // simulate work
            resources[1]->inUse = false;
            resources[1]->mtx.unlock();
            break;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100)); // keep checking
    }

    waiting = false;

    if (killed) {
        std::cout << "[Process " << id << "] was killed. Releasing Resource " << resources[0]->id << std::endl;
        resources[0]->inUse = false;
        resources[0]->mtx.unlock();
        return;
    }

    // Release first resource normally
    resources[0]->inUse = false;
    resources[0]->mtx.unlock();

    std::cout << "[Process " << id << "] done." << std::endl;
}

void Process::kill() {
    killed = true;
}

bool Process::isWaiting() const {
    return waiting;
}
