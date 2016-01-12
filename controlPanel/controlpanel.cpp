#include "controlpanel.h"
#include "ui_controlpanel.h"

ControlPanel::ControlPanel(QWidget *parent) : QWidget(parent), ui(new Ui::ControlPanel)
{
    ui->setupUi(this);
}

ControlPanel::~ControlPanel()
{
    delete ui;
}

void ControlPanel::arduinoConnectionChanged (ConnectionStatus status)
{
    arduinoReady = (bool)status;
    setGui();
}
void ControlPanel::phoneConnectionChanged   (ConnectionStatus status)
{
    phoneReady = (bool)status;
    setGui();
}
void ControlPanel::phoneAddressChanged      (QString ip, QString port)
{
    ui->ipAddress->setText(ip);
    ui->portNumber->setText(port);
}

void ControlPanel::scanningControler        (Instruction instruction)
{
    if(instruction == Instruction::endScanning)     //--zakończ skanowanie
        prepareForEndScanning();
    else                                            //--skanuj
    {
        if(currentPhotosNumber == 0)                        //--zrób pierwsze zdjęcie
            prepareForStartScanning();
        else if(currentPhotosNumber < chosenPhotosNumber)   //--rób kolejne dopóki nie zrobisz wszyskich
            prepareForTakeNextPhoto();
        else                                                //--wtedy zakończ skanowanie
            prepareForEndScanning();
    }
}

void ControlPanel::on_photosSlider_valueChanged         (int value)
{
    ui->photosLCD->display(availableAngles.at(0).at(value));
}
void ControlPanel::on_reconnectWithUsb_clicked          ()
{
    emit openConnectionArduino();
}
void ControlPanel::on_start_stop_clicked                ()
{
    const QString startLabel    {"Start"};
    const QString endLabel      {"End"};

    if(ui->start_stop->text() == startLabel)
    {
        scanningControler(Instruction::takePhoto);
        ui->start_stop->setText(endLabel);
    }
    else
        scanningControler(Instruction::endScanning);
}
void ControlPanel::on_scanningMethod_currentIndexChanged(int index)
{
    emit scanningModeChanged( index ? ScanningMode::photogrammetry : ScanningMode::laser );
}

void ControlPanel::prepareForStartScanning  ()
{
    setGui(GuiMode::whileScanning);

    int motorStepsNumber = availableAngles.at(1).at(ui->photosSlider->value());
    emit motorStepsNumberChanged(motorStepsNumber);

    chosenPhotosNumber = ui->photosLCD->intValue();
    currentPhotosNumber++;
    angleForRotation = motorStepsNumber * 0.7;
    currentAngle += angleForRotation;

    emit tableAngleChanged(currentAngle);
    emit sendInstructionArduino(Instruction::takePhoto);

    emit scanningStarted();
    emit message(MessageType::log, "Skanowanie rozpoczęte, zrobiono pierwsze zdjęcie");
}
void ControlPanel::prepareForTakeNextPhoto  ()
{
    currentPhotosNumber++;
    currentAngle += angleForRotation;

    emit tableAngleChanged(currentAngle);
    emit sendInstructionArduino(Instruction::takePhoto);

    QByteArray currentPhoto = QByteArray::number(currentPhotosNumber);
    QByteArray chosenPhotos = QByteArray::number(chosenPhotosNumber);
    emit message(MessageType::log, "Skanowanie w toku: "+currentPhoto+" / "+chosenPhotos);
}
void ControlPanel::prepareForEndScanning    ()
{
    setGui(GuiMode::afterScanning);

    emit sendInstructionArduino(Instruction::endScanning);
    emit scanningFinished();
    emit message(MessageType::log, "Skanowanie zakończone");
}

void ControlPanel::setGui                   (GuiMode mode)
{
    if(mode == GuiMode::whileScanning)
    {
        bool b{true};
        ui->photosSlider  ->setDisabled(b);
        ui->scanningMethod->setDisabled(b);
    }
    else if(mode == GuiMode::afterScanning)
    {
        bool b{phoneReady && arduinoReady};
        ui->start_stop->setText("Start");
        ui->start_stop      ->setEnabled(b);
        ui->photosSlider    ->setEnabled(b);
        ui->scanningMethod  ->setEnabled(b);

        ui->reconnectWithUsb  ->setDisabled(arduinoReady);
    }

    ui->arduinoStatusLabel->setText(connectedToQString(arduinoReady));
    ui->phoneStatusLabel->setText(connectedToQString(phoneReady));
}
QString ControlPanel::connectedToQString    (bool isConnected)
{
    return isConnected ? "Connected" : "Disconnected";
}

