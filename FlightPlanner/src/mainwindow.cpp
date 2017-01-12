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

   ui->Height1->setMaximum(200);
   ui->Height1->setSuffix("m");
   ui->Height2->setMaximum(200);
   ui->Height2->setSuffix("m");
   setWindowTitle("Flightplanner");

   ui->statusBar->setStyleSheet("color: blue;");

   if ( port->SerialIsOpen )
   {
      ui->statusBar->showMessage("Connected with COM3");
   }
   else
   {
      ui->statusBar->showMessage("Disconnected");
      ui->pushButton_3->setEnabled(false);
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
   // Send Alt2
   DataToSend.setNum(ui->Height1->value());
   qDebug() << DataToSend;
   port->SetData(DataToSend);
   port->send();

   if(!(SerialSucceed = port->ERROR > 0 ? true : false)){
      ui->statusBar->showMessage("Failed to send altitudes");
      return;
   }


   // Send Alt 1
   DataToSend.setNum(ui->Height2->value());
   qDebug() << DataToSend;
   port->SetData(DataToSend);
   port->send();

   if(!(SerialSucceed = port->ERROR > 0 ? true : false)){
      ui->statusBar->showMessage("Failed to send altitudes");
      return;
   }

   ui->statusBar->showMessage("Altitudes uploaded!");
//   SendCompleted sendcompleted;
//   sendcompleted.setModal(true);
//   sendcompleted.SendSucceed = SerialSucceed;
//   sendcompleted.exec();
}

void MainWindow::on_connectButton_clicked()
{
    if(!port->open()){
       ui->statusBar->showMessage("Failed to connect");
    }
}
