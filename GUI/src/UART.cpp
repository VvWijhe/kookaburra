#include "UART.h"
#include "mainwindow.h"


UART::UART(QObject *parent) : QObject(parent)
{
   serialPort  = new QSerialPort(this);
   serialPort->setBaudRate(QSerialPort::Baud115200,QSerialPort::AllDirections);
   serialPort->setDataBits(QSerialPort::Data8);
   serialPort->setParity(QSerialPort::NoParity);
   serialPort->setStopBits(QSerialPort::OneStop);
   //m_serialPort->setPortName("ttyAMA0");
   serialPort->setPortName("COM3");
   serialPort->setFlowControl(QSerialPort::NoFlowControl);

}

int UART::connectDevice(){
   if (!serialPort->open(QIODevice::ReadWrite)) {
      qDebug()<< QObject::tr("Failed to open port %1, error: %2").arg(serialPort->portName()).arg(serialPort->errorString());
      return 0;
   }
   else{
      qDebug()<<"Write";
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
   data = serialPort->readAll();

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


void UART::send(char *data)
{
   if (!serialPort->open(QIODevice::ReadWrite)) {
      qDebug()<< QObject::tr("Failed to open port %1, error: %2").arg(serialPort->portName()).arg(serialPort->errorString());
      return;
   }
   serialPort->write(data);
   serialPort->flush();
   serialPort->waitForBytesWritten(1000);
}
