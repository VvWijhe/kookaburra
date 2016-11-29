#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#define MINHEIGHT 10
#define MAXHEIGHT 200

#include <QMainWindow>
#include <UART.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QString buff;
    bool SerialSucceed;
    bool ok;
    int tempdat;
    QByteArray DataToSend;


private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

private:
    Ui::MainWindow *ui;
    UART *port;
};

#endif // MAINWINDOW_H
