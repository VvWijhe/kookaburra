/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionCom_gate_settings;
    QAction *actionAbout;
    QAction *actionCom_gate_settings_2;
    QAction *actionAbout_2;
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout_2;
    QLabel *PandaPic;
    QSplitter *splitter_2;
    QSplitter *splitter;
    QSpinBox *heightBox1;
    QSpinBox *heightBox2;
    QVBoxLayout *verticalLayout;
    QSplitter *splitter_3;
    QPushButton *pushButton;
    QPushButton *pushButton_2;
    QPushButton *pushButton_3;
    QPushButton *recButton;
    QMenuBar *menuBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(222, 447);
        actionCom_gate_settings = new QAction(MainWindow);
        actionCom_gate_settings->setObjectName(QStringLiteral("actionCom_gate_settings"));
        actionAbout = new QAction(MainWindow);
        actionAbout->setObjectName(QStringLiteral("actionAbout"));
        actionCom_gate_settings_2 = new QAction(MainWindow);
        actionCom_gate_settings_2->setObjectName(QStringLiteral("actionCom_gate_settings_2"));
        actionAbout_2 = new QAction(MainWindow);
        actionAbout_2->setObjectName(QStringLiteral("actionAbout_2"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        verticalLayout_2 = new QVBoxLayout(centralWidget);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        PandaPic = new QLabel(centralWidget);
        PandaPic->setObjectName(QStringLiteral("PandaPic"));
        PandaPic->setMinimumSize(QSize(200, 200));
        PandaPic->setPixmap(QPixmap(QString::fromUtf8("../../Desktop/school/Pr3Kookaburra/vliegendepanda.jpg")));
        PandaPic->setScaledContents(true);

        verticalLayout_2->addWidget(PandaPic);

        splitter_2 = new QSplitter(centralWidget);
        splitter_2->setObjectName(QStringLiteral("splitter_2"));
        splitter_2->setOrientation(Qt::Vertical);
        splitter = new QSplitter(splitter_2);
        splitter->setObjectName(QStringLiteral("splitter"));
        splitter->setOrientation(Qt::Vertical);
        heightBox1 = new QSpinBox(splitter);
        heightBox1->setObjectName(QStringLiteral("heightBox1"));
        heightBox1->setMinimum(10);
        heightBox1->setMaximum(300);
        heightBox1->setSingleStep(5);
        splitter->addWidget(heightBox1);
        heightBox2 = new QSpinBox(splitter);
        heightBox2->setObjectName(QStringLiteral("heightBox2"));
        heightBox2->setMinimum(10);
        heightBox2->setMaximum(300);
        heightBox2->setSingleStep(5);
        splitter->addWidget(heightBox2);
        splitter_2->addWidget(splitter);

        verticalLayout_2->addWidget(splitter_2);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        splitter_3 = new QSplitter(centralWidget);
        splitter_3->setObjectName(QStringLiteral("splitter_3"));
        splitter_3->setOrientation(Qt::Vertical);
        pushButton = new QPushButton(splitter_3);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        splitter_3->addWidget(pushButton);
        pushButton_2 = new QPushButton(splitter_3);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));
        splitter_3->addWidget(pushButton_2);
        pushButton_3 = new QPushButton(splitter_3);
        pushButton_3->setObjectName(QStringLiteral("pushButton_3"));
        splitter_3->addWidget(pushButton_3);
        recButton = new QPushButton(splitter_3);
        recButton->setObjectName(QStringLiteral("recButton"));
        splitter_3->addWidget(recButton);

        verticalLayout->addWidget(splitter_3);


        verticalLayout_2->addLayout(verticalLayout);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 222, 25));
        MainWindow->setMenuBar(menuBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0));
        actionCom_gate_settings->setText(QApplication::translate("MainWindow", "Com gate settings", 0));
        actionAbout->setText(QApplication::translate("MainWindow", "About", 0));
        actionCom_gate_settings_2->setText(QApplication::translate("MainWindow", "Com gate settings", 0));
        actionAbout_2->setText(QApplication::translate("MainWindow", "About", 0));
        PandaPic->setText(QString());
        pushButton->setText(QApplication::translate("MainWindow", "Com port settings", 0));
        pushButton_2->setText(QApplication::translate("MainWindow", "About", 0));
        pushButton_3->setText(QApplication::translate("MainWindow", "Send to Kookaburra", 0));
        recButton->setText(QApplication::translate("MainWindow", "Connect", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
