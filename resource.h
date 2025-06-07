#ifndef RESOURCE_H
#define RESOURCE_H

#include <mutex>

class Resource {
public:
    int id;
    bool inUse = false;
    std::mutex mtx;

    Resource(int id);
};

#endif // RESOURCE_H
