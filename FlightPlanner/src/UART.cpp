#include "UART.h"
#include "mainwindow.h"
#include <QDebug>


UART::UART(QObject *parent) : QObject(parent)
{
   serialPort  = new QSerialPort(this);
   serialPort->setBaudRate(QSerialPort::Baud115200,QSerialPort::AllDirections);
   serialPort->setDataBits(QSerialPort::Data8);
   serialPort->setParity(QSerialPort::NoParity);
   serialPort->setStopBits(QSerialPort::OneStop);
   //m_serialPort->setPortName("ttyAMA0");
   serialPort->setPortName("COM10");

   serialPort->setFlowControl(QSerialPort::NoFlowControl);

   if (!open()) {
      qDebug()<< QObject::tr("Failed to open port %1, error: %2").arg(serialPort->portName()).arg(serialPort->errorString());
      SerialIsOpen = false;
   }
   else{
      qDebug()<<"Write";
      SerialIsOpen = true;
      connect(serialPort,SIGNAL(readyRead()),this,SLOT(dataIn()));
      connect(serialPort,SIGNAL(error(QSerialPort::SerialPortError)),this,SLOT(handleError(QSerialPort::SerialPortError)));
   }
}

void UART::SetBaud(int baudRate)
{
   if (baudRate==115200){
      serialPort->setBaudRate(QSerialPort::Baud115200,QSerialPort::AllDirections);}
   if(baudRate==19200){
      serialPort->setBaudRate(QSerialPort::Baud19200,QSerialPort::AllDirections);}
}

bool UART::open(){
   return serialPort->open(QIODevice::ReadWrite);
}

bool UART::isSerialOpen()
{
   if(serialPort->isOpen()){
      serialPort->close();
      return true;
   }
   else{
      qDebug()<<"Attempting to open serialPort again..";
      if (!serialPort->open(QIODevice::ReadWrite)) {
         qDebug()<< QObject::tr("Failed to open port %1, error: %2").arg(serialPort->portName()).arg(serialPort->errorString());
      }
      else{
         qDebug()<<"Succesfully opend serialport";
      }
      return false;}
}

void UART::dataIn()
{
   qDebug()<<"datain started";
   qDebug() << serialPort->readAll();

}

void UART::handleError(QSerialPort::SerialPortError serialPortError)
{if (serialPortError == QSerialPort::ReadError) {
      qDebug()<< QObject::tr("An I/O error occurred while reading the data from port %1, error: %2").
                 arg(serialPort->portName()).arg(serialPort->errorString());
   }

}


void UART::closeSerial(void)
{
   serialPort->close();
}


void UART::send()
{
   ERROR = serialPort->write(data);
   qDebug()<<ERROR<<"error:";
//   serialPort->flush();
//   serialPort->waitForBytesWritten(1000);
}

void UART::sendString(QString str){
   serialPort->write(str.toStdString().c_str(), str.size());
}

void UART::SetData(QByteArray value)
{
   data = value;
}
