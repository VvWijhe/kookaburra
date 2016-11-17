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
#include <QtWidgets/QToolBar>
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
    QLabel *PandaPic;
    QWidget *layoutWidget;
    QVBoxLayout *verticalLayout;
    QSplitter *splitter_3;
    QPushButton *pushButton_2;
    QPushButton *pushButton_3;
    QSplitter *splitter_4;
    QLabel *label;
    QLabel *label_2;
    QSplitter *splitter_5;
    QSplitter *splitter_2;
    QSplitter *splitter;
    QSpinBox *Height1;
    QSpinBox *Height2;
    QSplitter *splitter_6;
    QLabel *Serialconnectedlabel;
    QLabel *label_3;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(325, 556);
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
        PandaPic = new QLabel(centralWidget);
        PandaPic->setObjectName(QStringLiteral("PandaPic"));
        PandaPic->setGeometry(QRect(10, 10, 200, 200));
        PandaPic->setPixmap(QPixmap(QString::fromUtf8("../../Desktop/school/Pr3Kookaburra/vliegendepanda.jpg")));
        PandaPic->setScaledContents(true);
        layoutWidget = new QWidget(centralWidget);
        layoutWidget->setObjectName(QStringLiteral("layoutWidget"));
        layoutWidget->setGeometry(QRect(10, 310, 201, 61));
        verticalLayout = new QVBoxLayout(layoutWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setSizeConstraint(QLayout::SetNoConstraint);
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        splitter_3 = new QSplitter(layoutWidget);
        splitter_3->setObjectName(QStringLiteral("splitter_3"));
        splitter_3->setOrientation(Qt::Vertical);
        pushButton_2 = new QPushButton(splitter_3);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));
        splitter_3->addWidget(pushButton_2);

        verticalLayout->addWidget(splitter_3);

        pushButton_3 = new QPushButton(layoutWidget);
        pushButton_3->setObjectName(QStringLiteral("pushButton_3"));

        verticalLayout->addWidget(pushButton_3);

        splitter_4 = new QSplitter(centralWidget);
        splitter_4->setObjectName(QStringLiteral("splitter_4"));
        splitter_4->setGeometry(QRect(100, 220, 47, 71));
        splitter_4->setOrientation(Qt::Vertical);
        label = new QLabel(splitter_4);
        label->setObjectName(QStringLiteral("label"));
        splitter_4->addWidget(label);
        label_2 = new QLabel(splitter_4);
        label_2->setObjectName(QStringLiteral("label_2"));
        splitter_4->addWidget(label_2);
        splitter_5 = new QSplitter(centralWidget);
        splitter_5->setObjectName(QStringLiteral("splitter_5"));
        splitter_5->setGeometry(QRect(10, 220, 81, 71));
        splitter_5->setOrientation(Qt::Vertical);
        splitter_2 = new QSplitter(splitter_5);
        splitter_2->setObjectName(QStringLiteral("splitter_2"));
        splitter_2->setOrientation(Qt::Vertical);
        splitter = new QSplitter(splitter_2);
        splitter->setObjectName(QStringLiteral("splitter"));
        splitter->setOrientation(Qt::Vertical);
        splitter_2->addWidget(splitter);
        splitter_5->addWidget(splitter_2);
        Height1 = new QSpinBox(splitter_5);
        Height1->setObjectName(QStringLiteral("Height1"));
        splitter_5->addWidget(Height1);
        Height2 = new QSpinBox(splitter_5);
        Height2->setObjectName(QStringLiteral("Height2"));
        splitter_5->addWidget(Height2);
        splitter_6 = new QSplitter(centralWidget);
        splitter_6->setObjectName(QStringLiteral("splitter_6"));
        splitter_6->setGeometry(QRect(10, 380, 172, 71));
        splitter_6->setOrientation(Qt::Vertical);
        Serialconnectedlabel = new QLabel(splitter_6);
        Serialconnectedlabel->setObjectName(QStringLiteral("Serialconnectedlabel"));
        splitter_6->addWidget(Serialconnectedlabel);
        label_3 = new QLabel(splitter_6);
        label_3->setObjectName(QStringLiteral("label_3"));
        splitter_6->addWidget(label_3);
        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 325, 26));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
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
        pushButton_2->setText(QApplication::translate("MainWindow", "About", 0));
        pushButton_3->setText(QApplication::translate("MainWindow", "Send to Kookaburra", 0));
        label->setText(QApplication::translate("MainWindow", "Height 1", 0));
        label_2->setText(QApplication::translate("MainWindow", "Height 2", 0));
        Serialconnectedlabel->setText(QApplication::translate("MainWindow", "TextLabel", 0));
        label_3->setText(QApplication::translate("MainWindow", "Height minimum value is 10.\n"
"Height maximum value is 200.", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
