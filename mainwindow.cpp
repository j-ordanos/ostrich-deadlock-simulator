#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Create emitter and hook signal
    emitter = new LogEmitter(this);
    // connect(sim, &Simulation::deadlockDetected, this, &MainWindow::on_deadlockDetected);
    connect(emitter, &LogEmitter::newLogMessage, this, &MainWindow::appendLogMessage);
    // Attach logStream
    logStream = new LogStream(emitter);
}

MainWindow::~MainWindow()
{
    delete logStream;
    delete sim;
    delete ui;
}

void MainWindow::appendLogMessage(const QString &message)
{
    ui->logOutput->append(message);
}

void MainWindow::on_startButton_clicked()
{
    if (!sim) {
        sim = new Simulation(this);  // Pass `this` as parent for proper QObject tracking
        connect(sim, &Simulation::deadlockDetected, this, &MainWindow::on_deadlockDetected);
        sim->run();
    }
}


void MainWindow::on_killP1Button_clicked()
{
    if (sim) {
        sim->killProcess(1);
        appendLogMessage("[User] Killed Process 1 to resolve deadlock.");
    }

    ui->killP1Button->setEnabled(false);
    ui->killP2Button->setEnabled(false);
}

void MainWindow::on_killP2Button_clicked()
{
    if (sim) {
        sim->killProcess(2);
        appendLogMessage("[User] Killed Process 2 to resolve deadlock.");
    }

    ui->killP1Button->setEnabled(false);
    ui->killP2Button->setEnabled(false);
}

void MainWindow::on_deadlockDetected()
{
    appendLogMessage("[System] FREEZE..... Please choose a process to kill.");
    ui->killP1Button->setEnabled(true);
    ui->killP2Button->setEnabled(true);
}
