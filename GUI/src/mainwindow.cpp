#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "mywindow.h"
#include "secondwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
   QMainWindow(parent),
   ui(new Ui::MainWindow)
{
   ui->setupUi(this);
   this->setFixedSize(300, 400);

   port = new UART;

   ui->pushButton_3->setEnabled(false);

   if(port->connectDevice()){
      ui->statusBar->showMessage("Device connected");
      ui->pushButton_3->setEnabled(true);
   } else {
      ui->statusBar->showMessage("Device not connected");
   }
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
   // Send height 1
   Height1 = ui->heightBox1->value();
   qDebug() << "Height 1: " << Height1;
   DataToSend = (char)Height1;
   port->send(&DataToSend);

   // Send height 2
   Height2 = ui->heightBox2->value();;
   qDebug() << "Height 2: " << Height2;
   DataToSend = (char)Height2;
   port->send(&DataToSend);
}

void MainWindow::on_recButton_clicked()
{
    if(port->connectDevice()){
       ui->statusBar->showMessage("Device connected");
       ui->pushButton_3->setEnabled(true);
    } else {
       ui->statusBar->showMessage("Device not connected");
    }
}
