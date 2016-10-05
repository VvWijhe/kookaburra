#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "mywindow.h"
#include "secondwindow.h"
#include <QDebug>
#include "UART.h"

UART port;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked() //Com Port settings
{
    MyWindow mywindow;
    mywindow.setModal(true);
    mywindow.exec();
}

void MainWindow::on_pushButton_2_clicked() //About
{
    SecondWindow secondwindow;
    secondwindow.setModal(true);
    secondwindow.exec();
}

void MainWindow::on_pushButton_3_clicked() //Send heightdata
{
    buff = ui->UIHeightOne->text();
    Height1 = buff.toInt(&ok, 10);
    if ( Height1 < MINHEIGHT )
    {
        Height1 = MINHEIGHT;
    }
    else if ( Height1 > MAXHEIGHT )
    {
        Height1 = MAXHEIGHT;
    }
    qDebug()<<Height1;
    *DataToSend = (char)Height1;
    port.send(DataToSend);


    buff = ui->UIHeightTwo->text();
    Height2 = buff.toInt(&ok, 10);
    if ( Height2 <MINHEIGHT )
    {
        Height2 = MINHEIGHT;
    }
    else if ( Height2 > MAXHEIGHT )
    {
        Height2 = MAXHEIGHT;
    }
    qDebug()<<Height2;
    *DataToSend = (char)Height2;
    port.send(DataToSend);
}
