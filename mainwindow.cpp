#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->scene->setFocus();
    openLogFile();

    connect(&phone, &TcpServer::connectionStatusChanged, ui->controlPanel, &ControlPanel::phoneConnectionChanged);
    connect(&phone, &TcpServer::receivedInstruction,     ui->controlPanel, &ControlPanel::scanningControler);
    connect(&phone, &TcpServer::serverAddressChanged,    ui->controlPanel, &ControlPanel::phoneAddressChanged);
    connect(&phone, &TcpServer::receivedPhotogram,       ui->scene,        &Scene::nextScan);
    connect(&phone, &TcpServer::message,                 this,             &MainWindow::messageHandling);

    connect(&arduino, &UsbPort::connectionStatusChanged, ui->controlPanel,  &ControlPanel::arduinoConnectionChanged);
    connect(&arduino, &UsbPort::receivedInstruction,    &phone,             &TcpServer::sendInstruction);
    connect(&arduino, &UsbPort::message,                 this,              &MainWindow::messageHandling);

    connect(ui->controlPanel, &ControlPanel::closeConnectionArduino, &arduino, &UsbPort::closeConnection);
    connect(ui->controlPanel, &ControlPanel::sendInstructionArduino, &arduino, &UsbPort::sendInstruction);
    connect(ui->controlPanel, &ControlPanel::setRotationAngle,       &arduino, &UsbPort::setRotateNumber);

    connect(ui->controlPanel, &ControlPanel::closeConnectionPhone,  &phone, &TcpServer::closeConnection);
    connect(ui->controlPanel, &ControlPanel::sendInstructionPhone,  &phone, &TcpServer::sendInstruction);
    connect(ui->controlPanel, &ControlPanel::angleChanged,          &phone, &TcpServer::angleChanged);

    connect(ui->controlPanel, &ControlPanel::scanningStarted,   ui->scene, &Scene::scanningStarted);
    connect(ui->controlPanel, &ControlPanel::scanningFinished,  ui->scene, &Scene::scanningFinished);
    connect(ui->controlPanel, &ControlPanel::message,           this,      &MainWindow::messageHandling);

    connect(ui->scene, &Scene::removeOldestPhotogram, &phone, &TcpServer::removeOldestPhotogram);

    phone.openConnection();
    arduino.openConnection();
}

MainWindow::~MainWindow()
{
    delete ui;

    closeLogFile();

    //--usuń pliki tymczasowe
}

void MainWindow::on_actionOpen_triggered()
{
    modelFilePath = QFileDialog::getOpenFileName(
                this, tr("Open Model3D"),
                QDir::homePath(), tr("Model3D (*.obj *.off)") );

    if( modelFilePath.isEmpty() )
        return;
    else
    {
        ui->scene->renderModelFromFile(modelFilePath);
        messageHandling(MessageType::log, "Otworzono plik z modelem: "+modelFilePath);
    }

}
void MainWindow::on_actionSave_triggered()
{
    QFile model{modelFilePath};
    QString modelFileName {model.fileName()};

    QFileDialog savePanel;
    savePanel.selectFile(modelFileName);

    QString newFileName = QFileDialog::getSaveFileName
            (
                this,
                "Save model",
                modelFilePath,
                tr("Model3D (*off)")
            );

    if(newFileName.isEmpty()) //--użytkownik kliknął cancel
        return;
    else
    {
        if(model.rename(newFileName))
            messageHandling(MessageType::log, "Plik z modelem został zapisany w: "+ newFileName);
        else
            messageHandling(MessageType::warning, "Plik z modelem nie został zapisany");
    }
}
void MainWindow::on_actionExit_triggered()
{
    qApp->exit(0);
    //--dodaj operacje przed zakmnięciem
}
void MainWindow::on_actionManual_triggered()
{
    //--manual którego nie ma i szybko nie będzie
}

void MainWindow::messageHandling(MessageType type, QString what, QString who)
{
    if(logFile.isWritable())
    {
        QString time = date.currentDateTime().toString("HH:mm:ss");
        QString log = time + "\t" + (char)type + "\t" + who + "\t" + what + "\n";

        stream<<log;
    }
    else
    {
        showMessageOnStatusBar("Nie można otworzyć dziennika zdarzeń");
        return;
    }

    if(type == MessageType::criticalError || type == MessageType::warning)
        showMessageOnStatusBar(what);


}
void MainWindow::openLogFile()
{
    if( logFile.open(QIODevice::WriteOnly) )
    {

        QString currentDate = date.date().toString("d.M.yyyy");
        QString currentTime = date.time().toString("HH:mm:ss");
        QString header = "\t\t Dziennik zdarzeń programu ScannerPC\t" + currentTime + "\t\t" + currentDate + "\n\n"+
        "Opis błędów: \n\n"
        "l:\tlog\t\t\t-\tinformacje które służą jedynie do oceny pracy programu\n"
        "w:\twarning\t\t-\tostrzeżenie, informacja o problemie\n"
        "r:\treceivedError\t-\tbłąd, problem otrzymany z UsbPort lub TcpClient\n"
        "i:\tinternalError\t-\tbłąd, problem wewnątrz klasy Qt\n"
        "c:\tcriticalError\t-\tbłąd, który bezpośrednio wpływa na pracę programu\n\n\n";

        stream << header;
    }
    else
        messageHandling(MessageType::criticalError, "Nie można otworzyć dziennika zdarzeń");
}
void MainWindow::closeLogFile()
{
    logFile.close();
}
void MainWindow::modelPathChanged(QString path)
{
    modelFilePath = path;
}
void MainWindow::showMessageOnStatusBar(QString what)
{
    ui->statusBar->showMessage(what, 3000);
}
