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

void ControlPanel::arduinoConnectionChanged(ConnectionStatus status)
{
    arduinoReady = (bool)status;
    setGui();
}
void ControlPanel::phoneConnectionChanged(ConnectionStatus status)
{
    phoneReady = (bool)status;
    setGui();
}
void ControlPanel::phoneAddressChanged(QString ip, QString port)
{
    ui->ipAddress->setText(ip);
    ui->portNumber->setText(port);
}

void ControlPanel::scanningControler(Instruction instruction)
{
    if(instruction == Instruction::endScanning)     //--zakończ skanowanie
        prepareForEndScanning();
    else                                            //--skanuj
    {
        if(automaticMode)                               //--w trybie wielu zdjęć
        {
            if(currentPhotosNumber == 0)                        //--zrób pierwsze zdjęcie
                prepareForStartScanning();
            else if(currentPhotosNumber < chosenPhotosNumber)   //--rób kolejne dopóki nie zrobisz wszyskich
                prepareForTakeNextPhoto();
            else                                                //--wtedy zakończ skanowanie
                prepareForEndScanning();
        }
        else                                            //--w trybie jednego zdjęcia
            prepareForTakeOnePhoto();
    }
}

void ControlPanel::on_automaticMode_clicked(bool checked)
{
    ui->start_stop->setText( checked ? startLabel : takePhotoLabel );
    automaticMode = checked;
}
void ControlPanel::on_photosSlider_valueChanged(int value)
{
    ui->photosLCD->display(availableAngles.at(0).at(value));
}
void ControlPanel::on_thresholdSlider_valueChanged(int value)
{
    ui->thresholdLCD->display(value);
}
void ControlPanel::on_connectWithUsb_clicked()
{
    emit openConnectionArduino();
}
void ControlPanel::on_start_stop_clicked()
{
    QString label{ui->start_stop->text()};

    if(label == takePhotoLabel || label == startLabel)
        scanningControler(Instruction::takePhoto);
    else
        scanningControler(Instruction::endScanning);
}

void ControlPanel::prepareForStartScanning()
{
    setGui(GuiMode::whileScanning);

    chosenPhotosNumber = ui->photosLCD->intValue();
    threshold = ui->thresholdLCD->intValue();

    float angleWithoutMotorStep07 = availableAngles.at(1).at(ui->photosSlider->value());
    angleForRotation = 0.7 * angleWithoutMotorStep07;
    currentPhotosNumber++;

    emit setRotationAngle(angleForRotation);
    emit sendInstructionArduino(Instruction::takePhoto);

    emit scanningStarted();
    emit message(MessageType::log, "Skanowanie rozpoczęte, zrobiono pierwsze zdjęcie");
}
void ControlPanel::prepareForTakeNextPhoto()
{
    currentPhotosNumber++;
    currentAngle += angleForRotation;

    emit angleChanged(currentAngle);
    emit sendInstructionArduino(Instruction::takePhoto);

    QByteArray currentPhoto = QByteArray::number(currentPhotosNumber);
    QByteArray chosenPhotos = QByteArray::number(chosenPhotosNumber);
    emit message(MessageType::log, "Skanowanie w toku: "+currentPhoto+" / "+chosenPhotos);
}
void ControlPanel::prepareForEndScanning()
{
    setGui(GuiMode::afterScanning);

    emit sendInstructionArduino(Instruction::endScanning);
    emit scanningFinished();
    emit message(MessageType::log, "Skanowanie zakończone");
}
void ControlPanel::prepareForTakeOnePhoto()
{
    setGui(GuiMode::whileScanning);

    emit sendInstructionPhone(Instruction::takePhoto);
    emit message(MessageType::log, "Wykonano jedno zdjęcie");
}

void ControlPanel::setGui(GuiMode mode)
{
    if(mode == GuiMode::whileScanning)
    {
        bool b{true};
        ui->automaticMode   ->setDisabled(b);
        ui->photosSlider    ->setDisabled(b);
        ui->thresholdSlider ->setDisabled(b);
    }
    else
    {
        bool b{phoneReady && arduinoReady};
        ui->start_stop      ->setEnabled(b);
        ui->photosSlider    ->setEnabled(b);
        ui->thresholdSlider ->setEnabled(b);
        ui->automaticMode   ->setEnabled(b);

        ui->connectWithUsb  ->setDisabled(arduinoReady);
    }

    ui->arduinoStatusLabel->setText(connectedToQString(arduinoReady));
    ui->phoneStatusLabel->setText(connectedToQString(phoneReady));
}
QString ControlPanel::connectedToQString(bool isConnected)
{
    return isConnected ? "Connected" : "Disconnected";
}


