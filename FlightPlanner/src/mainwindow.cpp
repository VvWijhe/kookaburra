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

   if ( port->SerialIsOpen == true )
   {
      ui->statusBar->showMessage("Connected with COM3");
   }
   else
   {
      ui->statusBar->showMessage("WARNING: Connection failed.");
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
   qDebug() << port->ERROR << "error in MW:";

   SerialSucceed = port->ERROR > 0 ? true : false;


   // Send Alt 1
   DataToSend.setNum(ui->Height2->value());
   qDebug() << DataToSend;
   port->SetData(DataToSend);
   port->send();

   SerialSucceed = port->ERROR > 0 ? true : false;

   SerialSucceed == true ? ui->statusBar->showMessage("Altitudes sent!") : ui->statusBar->showMessage("Failed to send altitudes");

//   SendCompleted sendcompleted;
//   sendcompleted.setModal(true);
//   sendcompleted.SendSucceed = SerialSucceed;
//   sendcompleted.exec();
}
