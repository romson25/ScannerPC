#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFile>
#include <QFileDialog>
#include <QDateTime>

#include <TcpServer/tcpserver.h>
#include <UsbPort/usbport.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

signals:

public slots:
    void modelPathChanged(QString);

private slots:
    void on_actionOpen_triggered();
    void on_actionSave_triggered();
    void on_actionExit_triggered();
    void on_actionManual_triggered();

private:
    Ui::MainWindow *ui;

    TcpServer   phone{};
    UsbPort     arduino{};

    QDateTime   date{QDateTime::currentDateTime()};
    QFile       logFile{"logfile.txt"};
    QTextStream stream{&logFile};

    QString modelFilePath;

    void openLogFile();
    void closeLogFile();

    void messageHandling(MessageType type, QString what, QString who = "MainWindow");
    void showMessageOnStatusBar(QString what);
};

#endif // MAINWINDOW_H
