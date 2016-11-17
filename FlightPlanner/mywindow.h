#ifndef MYWINDOW_H
#define MYWINDOW_H

#include <QDialog>

namespace Ui {
class MyWindow;
}

class MyWindow : public QDialog
{
    Q_OBJECT

public:
    explicit MyWindow(QWidget *parent = 0);
    ~MyWindow();

private:
    Ui::MyWindow *ui;
};

#endif // MYWINDOW_H
