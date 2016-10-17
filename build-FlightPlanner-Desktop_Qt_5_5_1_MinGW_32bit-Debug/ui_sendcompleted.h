/********************************************************************************
** Form generated from reading UI file 'sendcompleted.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SENDCOMPLETED_H
#define UI_SENDCOMPLETED_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QTextBrowser>

QT_BEGIN_NAMESPACE

class Ui_SendCompleted
{
public:
    QTextBrowser *textBrowser;

    void setupUi(QDialog *SendCompleted)
    {
        if (SendCompleted->objectName().isEmpty())
            SendCompleted->setObjectName(QStringLiteral("SendCompleted"));
        SendCompleted->resize(388, 39);
        textBrowser = new QTextBrowser(SendCompleted);
        textBrowser->setObjectName(QStringLiteral("textBrowser"));
        textBrowser->setGeometry(QRect(0, 0, 391, 41));

        retranslateUi(SendCompleted);

        QMetaObject::connectSlotsByName(SendCompleted);
    } // setupUi

    void retranslateUi(QDialog *SendCompleted)
    {
        SendCompleted->setWindowTitle(QApplication::translate("SendCompleted", "Dialog", 0));
        textBrowser->setHtml(QApplication::translate("SendCompleted", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:7.8pt; font-weight:400; font-style:normal;\">\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><br /></p></body></html>", 0));
    } // retranslateUi

};

namespace Ui {
    class SendCompleted: public Ui_SendCompleted {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SENDCOMPLETED_H
