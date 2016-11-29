#ifndef UART_H
#define UART_H

#include <QObject>
#include <QSerialPort>
#include <QtCore>
#include <QDebug>


class UART : public QObject
{
    Q_OBJECT

public:
    Q_INVOKABLE explicit UART (QObject *parent=0);
    Q_INVOKABLE bool isSerialOpen();
    void closeSerial(void);
    void SetBaud(int baudRate);
    void SetData(QByteArray value);
    int ERROR;
    bool SerialIsOpen;
public slots:
    void dataIn();
    void send();
    void handleError(QSerialPort::SerialPortError serialPortError);
private:
    QSerialPort *serialPort;
    QByteArray data;
    QString datasend;
};

#endif // UART_H
