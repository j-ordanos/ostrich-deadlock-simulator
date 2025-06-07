#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "simulation.h"  // Include your Simulation class
#include "logstream.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

#include "logstream.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_startButton_clicked();
    void appendLogMessage(const QString& message);  // NEW SLOT

    void on_killP1Button_clicked();  // NEW
    void on_killP2Button_clicked();  // NEW
    void on_deadlockDetected();  // NEW

private:
    Ui::MainWindow *ui;
    Simulation* sim = nullptr;
    LogEmitter* emitter = nullptr;
    LogStream* logStream = nullptr;
};


#endif // MAINWINDOW_H

