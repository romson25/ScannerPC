#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFile>
#include <QFileDialog>
#include <QDebug>
#include <QDateTime>

#include <TcpServer/tcpserver.h>
#include <UsbPort/usbport.h>

#include "controlPanel/controlpanel.h"
#include "controlPanel/scanningmode.h"
#include "renderWindow/scene.h"
#include "reconstructor/laserreconstructor.h"
#include "reconstructor/photogrammetryreconstructor.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void imageFlowControl   (QImage &);
    void scanningStarted    ();
    void scanningFinished   ();
    void setScanningMode    (ScanningMode);

private slots:
    void on_actionOpen_triggered();
    void on_actionSave_triggered();
    void on_actionExit_triggered();
    void on_actionHelp_triggered();

private:
    Ui::MainWindow *ui;

    TcpServer   phone   {};
    UsbPort     arduino {};

    LaserReconstructor          laserReconstructor {};
    PhotogrammetryReconstructor photogrammetryReconstructor {};

    QDateTime   date    { QDateTime::currentDateTime() };
    QFile       logFile { "logfile.txt" };
    QTextStream stream  { &logFile };

    QString modelFilePath;
    ScanningMode scanningMode {ScanningMode::laser};

    void openLogFile    ();
    void closeLogFile   ();
    void messageHandling        (MessageType type, QString what, QString who = "MainWindow");
    void showMessageOnStatusBar (QString what);
};

#endif // MAINWINDOW_H
