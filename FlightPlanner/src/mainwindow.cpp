#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "sendcompleted.h"
#include "secondwindow.h"
#include <QDebug>
#include "UART.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    port = new UART;
    ui->Height1->setMaximum(999);
    ui->Height1->setSuffix("m");
    ui->Height2->setMaximum(999);
    ui->Height2->setSuffix("m");
    if ( port->SerialIsOpen == true )
    {
        ui->Serialconnectedlabel->setText("Connected with COM3");
    }
    else
    {
        ui->Serialconnectedlabel->setText("WARNING: Connection failed.");
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_2_clicked() //About
{
    SecondWindow secondwindow;
    secondwindow.setModal(true);
    secondwindow.exec();
}

void MainWindow::on_pushButton_3_clicked() //Send heightdata
{
    tempdat = ui->Height1->value();
    if ( tempdat <MINHEIGHT )
    {
        tempdat = MINHEIGHT;
    }
    else if ( tempdat > MAXHEIGHT )
    {
        tempdat = MAXHEIGHT;
    }
    DataToSend.setNum(tempdat);
    qDebug()<<DataToSend;
    port->SetData(DataToSend);
    port->send();
    qDebug()<<port->ERROR<<"error in MW:";
    if ( port->ERROR > 0 )
    {
        SerialSucceed = true;
    }
    else
    {
        SerialSucceed = false;
    }

    tempdat = ui->Height2->value();
    if ( tempdat <MINHEIGHT )
    {
        tempdat = MINHEIGHT;
    }
    else if ( tempdat > MAXHEIGHT )
    {
        tempdat = MAXHEIGHT;
    }
    DataToSend.setNum(tempdat);
    qDebug()<<DataToSend;
    port->SetData(DataToSend);
    port->send();
    if ( port->ERROR > 0 && SerialSucceed == true )
    {
        SerialSucceed = true;
    }
    else
    {
        SerialSucceed = false;
    }

    SendCompleted sendcompleted;
    sendcompleted.setModal(true);
    sendcompleted.SendSucceed = SerialSucceed;
    sendcompleted.exec();
}
