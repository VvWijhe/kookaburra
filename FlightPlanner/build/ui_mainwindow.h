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
#include <QtWidgets/QHBoxLayout>
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
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_2;
    QSpinBox *Height2;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QSpinBox *Height1;
    QVBoxLayout *verticalLayout;
    QPushButton *pushButton_3;
    QSplitter *splitter_3;
    QPushButton *pushButton_2;
    QMenuBar *menuBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(400, 500);
        MainWindow->setMinimumSize(QSize(400, 500));
        MainWindow->setMaximumSize(QSize(400, 500));
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
        PandaPic->setMinimumSize(QSize(100, 200));
        PandaPic->setPixmap(QPixmap(QString::fromUtf8("../../Desktop/school/Pr3Kookaburra/vliegendepanda.jpg")));
        PandaPic->setScaledContents(true);

        verticalLayout_2->addWidget(PandaPic);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(-1, 0, -1, -1);
        label_2 = new QLabel(centralWidget);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setMaximumSize(QSize(67, 16777215));

        horizontalLayout_2->addWidget(label_2);

        Height2 = new QSpinBox(centralWidget);
        Height2->setObjectName(QStringLiteral("Height2"));

        horizontalLayout_2->addWidget(Height2);


        verticalLayout_2->addLayout(horizontalLayout_2);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(-1, 0, -1, -1);
        label = new QLabel(centralWidget);
        label->setObjectName(QStringLiteral("label"));
        label->setMinimumSize(QSize(0, 0));
        label->setMaximumSize(QSize(67, 16777215));

        horizontalLayout->addWidget(label);

        Height1 = new QSpinBox(centralWidget);
        Height1->setObjectName(QStringLiteral("Height1"));

        horizontalLayout->addWidget(Height1);


        verticalLayout_2->addLayout(horizontalLayout);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setSizeConstraint(QLayout::SetFixedSize);
        pushButton_3 = new QPushButton(centralWidget);
        pushButton_3->setObjectName(QStringLiteral("pushButton_3"));
        pushButton_3->setMinimumSize(QSize(280, 0));
        pushButton_3->setMaximumSize(QSize(16777215, 34));

        verticalLayout->addWidget(pushButton_3);

        splitter_3 = new QSplitter(centralWidget);
        splitter_3->setObjectName(QStringLiteral("splitter_3"));
        splitter_3->setOrientation(Qt::Vertical);
        pushButton_2 = new QPushButton(splitter_3);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));
        pushButton_2->setMaximumSize(QSize(16777215, 34));
        splitter_3->addWidget(pushButton_2);

        verticalLayout->addWidget(splitter_3);


        verticalLayout_2->addLayout(verticalLayout);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 400, 25));
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
        label_2->setText(QApplication::translate("MainWindow", "Altitude 1", 0));
        label->setText(QApplication::translate("MainWindow", "Altitude 2", 0));
        pushButton_3->setText(QApplication::translate("MainWindow", "Send to Kookaburra", 0));
        pushButton_2->setText(QApplication::translate("MainWindow", "About", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
