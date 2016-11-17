#ifndef SENDCOMPLETED_H
#define SENDCOMPLETED_H

#include <QDialog>

namespace Ui {
class SendCompleted;
}

class SendCompleted : public QDialog
{
    Q_OBJECT

public:
    explicit SendCompleted(QWidget *parent = 0);
    ~SendCompleted();
    bool SendSucceed;

private:
    Ui::SendCompleted *ui;
};

#endif // SENDCOMPLETED_H
