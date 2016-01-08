#ifndef CONTROLPANEL_H
#define CONTROLPANEL_H

#include <QWidget>
#include <QDebug>

#include "guimode.h"
#include "DataProcessor/datatype.h"
#include "DataProcessor/instruction.h"
#include "DataProcessor/messagetype.h"
#include "Connection/connectionstatus.h"

namespace Ui {
class ControlPanel;
}

class ControlPanel : public QWidget
{
    Q_OBJECT

public:
    explicit ControlPanel(QWidget *parent = 0);
    ~ControlPanel();

signals:
    void openConnectionArduino  ();
    void openConnectionPhone    ();
    void closeConnectionArduino ();
    void closeConnectionPhone   ();
    void sendInstructionArduino (Instruction);
    void sendInstructionPhone   (Instruction);

    void scanningStarted    ();
    void scanningFinished   ();
    void setRotationAngle   (float);
    void angleChanged       (float);
    void message            (MessageType, QString, QString = "ControlPanel");

public slots:
    void arduinoConnectionChanged   (ConnectionStatus);

    void phoneConnectionChanged     (ConnectionStatus);
    void phoneAddressChanged        (QString, QString);

    void scanningControler(Instruction);

private slots:
    void on_automaticMode_clicked           (bool checked);
    void on_photosSlider_valueChanged       (int value);
    void on_thresholdSlider_valueChanged    (int value);
    void on_connectWithUsb_clicked          ();
    void on_start_stop_clicked              ();

private:
    Ui::ControlPanel *ui;

    QVector<QVector<int> > availableAngles {{2,4,8,16,32,64,128,256,512},{257,128,64,32,16,8,4,2,1}};

    int chosenPhotosNumber  {};
    int currentPhotosNumber {};
    int threshold           {};

    float currentAngle      {};
    float angleForRotation  {};

    bool arduinoReady   {};
    bool phoneReady     {};
    bool automaticMode  {};

    const QString startLabel     {"Start"};
    const QString endLabel       {"End"};
    const QString takePhotoLabel {"Take photo"};

    void prepareForStartScanning ();
    void prepareForTakeNextPhoto ();
    void prepareForEndScanning   ();
    void prepareForTakeOnePhoto  ();

    void setGui(GuiMode = GuiMode::afterScanning);
    QString connectedToQString(bool);
};

#endif // CONTROLPANEL_H
