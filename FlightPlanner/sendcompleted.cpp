#include "sendcompleted.h"
#include "ui_sendcompleted.h"

SendCompleted::SendCompleted(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SendCompleted)
{
    ui->setupUi(this);
    if ( SendSucceed == true )
    {
        ui->textBrowser->append("The data was sent successfully.");
    }
    else
    {
        ui->textBrowser->append("The data wasn't sent successfully. Please check the \"about\" page for more information. ");
    }
}

SendCompleted::~SendCompleted()
{
    delete ui;
}
