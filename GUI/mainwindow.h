#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#define MINHEIGHT 10
#define MAXHEIGHT 300

#include <QMainWindow>

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
    bool ok;
    char *DataToSend;

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
