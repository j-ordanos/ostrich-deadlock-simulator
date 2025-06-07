#ifndef SIMULATION_H
#define SIMULATION_H

#include <vector>
#include <thread>
#include "process.h"
#include "resource.h"
#include <QObject>


class Simulation : public QObject
{
    Q_OBJECT

public:
    explicit Simulation(QObject* parent = nullptr);
    ~Simulation();

    void run();
    void killProcess(int processId);

signals:
    void deadlockDetected();  // NEW SIGNAL

private:
    Resource* R1;
    Resource* R2;
    Process* P1;
    Process* P2;
    std::thread t1;
    std::thread t2;
};


#endif // SIMULATION_H
