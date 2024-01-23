/********************************************************************************
** Form generated from reading UI file 'progressviewer.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PROGRESSVIEWER_H
#define UI_PROGRESSVIEWER_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ProgressViewer
{
public:
    QVBoxLayout *verticalLayout;
    QWidget *container;
    QVBoxLayout *verticalLayout_2;
    QLabel *label;
    QProgressBar *progress;

    void setupUi(QWidget *ProgressViewer)
    {
        if (ProgressViewer->objectName().isEmpty())
            ProgressViewer->setObjectName(QString::fromUtf8("ProgressViewer"));
        ProgressViewer->resize(243, 77);
        ProgressViewer->setStyleSheet(QString::fromUtf8("#container {\n"
"	background-color: rgb(57, 63, 77);\n"
"}"));
        verticalLayout = new QVBoxLayout(ProgressViewer);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        container = new QWidget(ProgressViewer);
        container->setObjectName(QString::fromUtf8("container"));
        container->setStyleSheet(QString::fromUtf8("#label {\n"
"	color: rgb(255, 255, 255);\n"
"   \n"
"	font: 16pt \"Ubuntu\";\n"
"}\n"
"\n"
"#progress {\n"
"  background-color:rgba(255, 255, 255, 0.2);\n"
"  border-radius: 4px;\n"
"}\n"
"\n"
"#progress::chunk\n"
"{\n"
"   background-color:rgba(255, 255, 255, 0.6);\n"
"   border-radius: 4px;\n"
"}\n"
""));
        verticalLayout_2 = new QVBoxLayout(container);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        label = new QLabel(container);
        label->setObjectName(QString::fromUtf8("label"));
        label->setAlignment(Qt::AlignCenter);

        verticalLayout_2->addWidget(label);

        progress = new QProgressBar(container);
        progress->setObjectName(QString::fromUtf8("progress"));
        progress->setMinimumSize(QSize(0, 5));
        progress->setMaximumSize(QSize(16777215, 5));
        progress->setValue(24);

        verticalLayout_2->addWidget(progress);


        verticalLayout->addWidget(container);


        retranslateUi(ProgressViewer);

        QMetaObject::connectSlotsByName(ProgressViewer);
    } // setupUi

    void retranslateUi(QWidget *ProgressViewer)
    {
        ProgressViewer->setWindowTitle(QCoreApplication::translate("ProgressViewer", "Form", nullptr));
        label->setText(QCoreApplication::translate("ProgressViewer", "\350\247\206\351\242\221\346\213\267\350\264\235\344\270\255\302\267\302\267\302\267\302\267\302\267", nullptr));
        progress->setFormat(QString());
    } // retranslateUi

};

namespace Ui {
    class ProgressViewer: public Ui_ProgressViewer {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PROGRESSVIEWER_H
