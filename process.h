#ifndef PROCESS_H
#define PROCESS_H

#include <vector>
#include <thread>
#include "resource.h"

class Process {
public:
    int id;
    std::vector<Resource*> resources;

    Process(int id, const std::vector<Resource*>& resources);
    void run();          // logic for each thread
    void kill();         // allow external kill
    bool isWaiting() const;

private:
    bool killed = false;     // whether this process has been killed
    bool waiting = false;    // whether this process is in a waiting state
};

#endif // PROCESS_H
