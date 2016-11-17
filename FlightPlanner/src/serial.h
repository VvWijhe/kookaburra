#ifndef SERIAL_H
#define SERIAL_H

#include "mainwindow.h"
#include <QMainWindow>
#include <QWidget>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QDialog>
#include <QTimer>

class Serial : public QWidget
{
    Q_OBJECT

public:
    explicit Serial(QWidget *parent = 0);
    explicit Serial(QString CP, QString BR, QString DB, QString P, QString SB, QString FC);
    ~Serial();

    void SendData();
    void reInit();

    quint16 Panda_V2_H1; // Hoogte 1
    quint16 Panda_V2_H2; // Hoogte 2
    bool Reconnect;
    bool ReInit_Done;
    QString Panda_V2_Portname;

    void setCOMPort(QString COMPORT) {COMPort = COMPORT;}
    QString getCOMPort() const {return COMPort;}

private slots:
    void ReadData();
    void tmr_Send();

private:
    QTimer Timer_Send;

    QString COMPort;
    QString BaudRate;
    QString DataBits;
    QString Parity;
    QString StopBits;
    QString FlowControl;

    static const quint16 Panda_V2_VendorID  = 1027; // 4292
    static const quint16 Panda_V2_ProductID = 24577; // 60000

    QSerialPort *Panda_V2;

    bool Panda_V2_aviable;
    QByteArray received_data;

    static const quint8 STX  = 2;
    static const quint8 ETX  = 3;
    static const quint8 sBit = 1;
    static const quint8 ACK  = 6;
    static const quint8 NAK  = 21;
    quint8 NAK_count;

    quint8 Hoogte1_1;
    quint8 Hoogte1_2;
    quint8 Hoogte2_1;
    quint8 Hoogte2_2;
    quint8 Check_Sum;
    QByteArray serialData;
    QString serialBuffer;
};

#endif // SERIAL_H
