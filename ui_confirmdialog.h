/********************************************************************************
** Form generated from reading UI file 'confirmdialog.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CONFIRMDIALOG_H
#define UI_CONFIRMDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ConfirmDialog
{
public:
    QHBoxLayout *horizontalLayout;
    QWidget *container;
    QVBoxLayout *verticalLayout;
    QLabel *title;
    QPushButton *accept_btn;
    QSpacerItem *verticalSpacer;
    QPushButton *reject_btn;

    void setupUi(QDialog *ConfirmDialog)
    {
        if (ConfirmDialog->objectName().isEmpty())
            ConfirmDialog->setObjectName(QString::fromUtf8("ConfirmDialog"));
        ConfirmDialog->resize(301, 183);
        horizontalLayout = new QHBoxLayout(ConfirmDialog);
        horizontalLayout->setSpacing(4);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        container = new QWidget(ConfirmDialog);
        container->setObjectName(QString::fromUtf8("container"));
        container->setStyleSheet(QString::fromUtf8("#container {\n"
"	background-color:rgba(57, 63, 77, 1);\n"
"}\n"
"\n"
"#title {\n"
"	color: rgb(255, 255, 255);\n"
"	font: 16pt \"Ubuntu\";\n"
"}\n"
"\n"
"QPushButton {\n"
"	background-color: rgba(39, 42, 51, 1);\n"
"	color: rgb(255, 255, 255);\n"
"    border:0px;\n"
"	font: 16pt \"Ubuntu\";\n"
"}\n"
"\n"
"QPushButton::pressed{\n"
"	background-color: rgba(255, 255, 255, 1);\n"
"	color: rgb(0, 0, 0);\n"
"    border:0px;\n"
"	font: 16pt \"Ubuntu\";\n"
"}"));
        verticalLayout = new QVBoxLayout(container);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(10, 0, 10, 10);
        title = new QLabel(container);
        title->setObjectName(QString::fromUtf8("title"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(title->sizePolicy().hasHeightForWidth());
        title->setSizePolicy(sizePolicy);
        title->setMinimumSize(QSize(0, 50));
        title->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(title);

        accept_btn = new QPushButton(container);
        accept_btn->setObjectName(QString::fromUtf8("accept_btn"));
        QSizePolicy sizePolicy1(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(accept_btn->sizePolicy().hasHeightForWidth());
        accept_btn->setSizePolicy(sizePolicy1);
        accept_btn->setMinimumSize(QSize(0, 50));

        verticalLayout->addWidget(accept_btn);

        verticalSpacer = new QSpacerItem(20, 5, QSizePolicy::Minimum, QSizePolicy::Fixed);

        verticalLayout->addItem(verticalSpacer);

        reject_btn = new QPushButton(container);
        reject_btn->setObjectName(QString::fromUtf8("reject_btn"));
        sizePolicy1.setHeightForWidth(reject_btn->sizePolicy().hasHeightForWidth());
        reject_btn->setSizePolicy(sizePolicy1);
        reject_btn->setMinimumSize(QSize(0, 50));

        verticalLayout->addWidget(reject_btn);


        horizontalLayout->addWidget(container);


        retranslateUi(ConfirmDialog);
        QObject::connect(accept_btn, SIGNAL(clicked()), ConfirmDialog, SLOT(accept()));
        QObject::connect(reject_btn, SIGNAL(clicked()), ConfirmDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(ConfirmDialog);
    } // setupUi

    void retranslateUi(QDialog *ConfirmDialog)
    {
        ConfirmDialog->setWindowTitle(QCoreApplication::translate("ConfirmDialog", "Dialog", nullptr));
        title->setText(QCoreApplication::translate("ConfirmDialog", "TITLE", nullptr));
        accept_btn->setText(QCoreApplication::translate("ConfirmDialog", "\347\241\256\350\256\244", nullptr));
        reject_btn->setText(QCoreApplication::translate("ConfirmDialog", "\345\217\226\346\266\210", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ConfirmDialog: public Ui_ConfirmDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CONFIRMDIALOG_H
