#include "serial.h"
#include "mainwindow.h"
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QtWidgets>
#include <QWidget>
#include <QMessageBox>
#include <QDebug>

#include <iostream>
#include <fstream>
#include <string>
#include <cctype>

#include <QtCore>
#include <QTime>
#include <QTimer>

Serial::Serial(QWidget *parent)
    : QWidget(parent)
{
    Panda_V2_aviable = false;
    ReInit_Done = false;
    Panda_V2_Portname = "";
    Panda_V2 = new QSerialPort;
    serialBuffer = "";
    NAK_count = 0;

    qDebug() << "Number of aviable ports: " << QSerialPortInfo::availablePorts().length();

    foreach(const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts())
    {
        if(serialPortInfo.hasVendorIdentifier())
        {
            qDebug() << "Vendor ID: " << serialPortInfo.vendorIdentifier();
        }
        if(serialPortInfo.hasProductIdentifier())
        {
            qDebug() << "Product ID: " << serialPortInfo.productIdentifier();
        }

        qDebug() << "Poort naam: " << serialPortInfo.portName();
    }
    foreach (const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts())
    {
        if(serialPortInfo.hasVendorIdentifier() && serialPortInfo.hasProductIdentifier())
        {
            if(serialPortInfo.vendorIdentifier() == Panda_V2_VendorID)
            {
                if(serialPortInfo.productIdentifier() == Panda_V2_ProductID)
                {
                    Panda_V2_Portname = serialPortInfo.portName();
                    Panda_V2_aviable = true;
                }
            }
        }
    }

    if(Panda_V2_aviable)
    {
        Panda_V2->setPortName(Panda_V2_Portname);
        Panda_V2->open(QSerialPort::ReadWrite);
        Panda_V2->setBaudRate(QSerialPort::Baud115200);
        Panda_V2->setDataBits(QSerialPort::Data8);
        Panda_V2->setParity(QSerialPort::NoParity);
        Panda_V2->setStopBits(QSerialPort::OneStop);
        Panda_V2->setFlowControl(QSerialPort::NoFlowControl);
        connect(Panda_V2, SIGNAL(readyRead()), this, SLOT(ReadData()));
        Reconnect = true;
    }
    else
    {
        //give error message if not aviable
        QMessageBox::warning(this, "Poort Error", "PandApp kan de Panda V2 niet vinden!");
        Reconnect = false;
    }
}

Serial::~Serial()
{
    qDebug() << "Destructor";
    if(Panda_V2->isOpen())
    {
        Panda_V2->close();
    }
}

void Serial::ReadData()
{
    Timer_Send.stop();
    serialData = Panda_V2->readAll();
    serialBuffer += QString::fromStdString(serialData.toStdString());

    // Getting a ACK
    if(serialBuffer[0] == ACK)
    {
        QMessageBox::information(this,  "Goed ontvangen!",
                                        "De ontvagen data is GOED ontvangen!");
    }
    // Getting a NAK
    else if(serialBuffer[0] == NAK)
    {
        NAK_count++;
        if(NAK_count >= 5)
        {
            QMessageBox::critical(this, "Data niet goed ontvangen!",
                                        "Er is 5 keer data ontvangen wat er op luidt dat er iets mis is! \n"
                                        "Reset het vliegtuig en probeer het nog een keer!");
            NAK_count = 0;
            serialBuffer = "";
        }
        else
        {
            QMessageBox::critical(this, "Data niet goed ontvangen!",
                                        "De ontvagen data is NIET goed ontvangen! \n"
                                        "Probeer het nog een keer!");
            serialBuffer = "";
        }

    }
    // Getting useless data
    else
    {
        QMessageBox::warning(this,  "Ongeldige data!",
                                    "De ontvagen data komt NIET bekend voor ! \n"
                                                        "Probeer het nog een keer!");
        serialBuffer = "";
    }
}

void Serial::SendData()
{
    Timer_Send.setSingleShot(true);
    connect(&Timer_Send, SIGNAL(timeout()), this, SLOT(tmr_Send()));
    Timer_Send.start(1000);

    //Hoogte 1
    Hoogte1_1 = (Panda_V2_H1 >> 8);
    Hoogte1_2 = Panda_V2_H1;

    //Hoogte 2
    Hoogte2_1 = ((Panda_V2_H2 & 0xFF00) >> 8);
    Hoogte2_2 = ( Panda_V2_H2 & 0x00FF);

    if(Panda_V2->isWritable())
    {
        qDebug() << "Panda_V2 is Writable";

        QByteArray SendBuffer = 0;
        quint8 i = 0;

        // Calculate the Checksum
        Check_Sum = 0;
        Check_Sum = STX + sBit + Hoogte1_1 + Hoogte1_2 + Hoogte2_1 + Hoogte2_2;

        SendBuffer[i] = ACK;
        //Send the data
        Panda_V2->write(SendBuffer);
    }
}

void Serial::reInit()
{
    qDebug() << "reInit: " << getCOMPort();

    if(Panda_V2->isOpen())
    {
        Panda_V2->close();
    }
    Panda_V2->setPortName(getCOMPort());
    Panda_V2->open(QSerialPort::ReadWrite);
}

void Serial::tmr_Send()
{
    Timer_Send.stop();
    QMessageBox::warning(this, "Time out!", "Er is geen geldige data ontvangen binnen de gewenste tijd. \n"
                                            "Probeer het versturen van data nog een keer!");
}

