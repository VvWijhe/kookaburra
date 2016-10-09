#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#define MINHEIGHT 10
#define MAXHEIGHT 300

#include <QMainWindow>
#include "UART.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    quint16 Height1;
    quint16 Height2;
    QString buff;
    char DataToSend;

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_recButton_clicked();

private:
    Ui::MainWindow *ui;
    UART *port;
};

#endif // MAINWINDOW_H
