#include "simulation.h"
#include <iostream>

Simulation::Simulation(QObject* parent) : QObject(parent)
{
    R1 = new Resource(1);
    R2 = new Resource(2);

    std::vector<Resource*> res1 = {R1, R2};
    std::vector<Resource*> res2 = {R2, R1};

    P1 = new Process(1, res1);
    P2 = new Process(2, res2);
}


Simulation::~Simulation() {
    if (t1.joinable()) t1.join();
    if (t2.joinable()) t2.join();

    delete P1;
    delete P2;
    delete R1;
    delete R2;
}

void Simulation::run() {
    std::cout << "[Simulation] Starting simulation..." << std::endl;

    t1 = std::thread(&Process::run, P1);
    t2 = std::thread(&Process::run, P2);

    // Start a watcher thread to detect if both are waiting
    std::thread([this]() {
        using namespace std::chrono_literals;
        while (true) {
            std::this_thread::sleep_for(100ms);
            if (P1 && P2 && P1->isWaiting() && P2->isWaiting()) {
                emit deadlockDetected();  // Notify GUI
                break;
            }
        }
    }).detach();
}


void Simulation::killProcess(int processId) {
    if (processId == 1 && P1) {
        P1->kill();
    } else if (processId == 2 && P2) {
        P2->kill();
    }
}
