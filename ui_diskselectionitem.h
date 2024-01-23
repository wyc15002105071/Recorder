/********************************************************************************
** Form generated from reading UI file 'diskselectionitem.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DISKSELECTIONITEM_H
#define UI_DISKSELECTIONITEM_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_DiskSelectionItem
{
public:
    QHBoxLayout *horizontalLayout;
    QPushButton *item_btn;

    void setupUi(QWidget *DiskSelectionItem)
    {
        if (DiskSelectionItem->objectName().isEmpty())
            DiskSelectionItem->setObjectName(QString::fromUtf8("DiskSelectionItem"));
        DiskSelectionItem->resize(236, 50);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(DiskSelectionItem->sizePolicy().hasHeightForWidth());
        DiskSelectionItem->setSizePolicy(sizePolicy);
        DiskSelectionItem->setMinimumSize(QSize(0, 50));
        DiskSelectionItem->setMaximumSize(QSize(16777215, 50));
        DiskSelectionItem->setStyleSheet(QString::fromUtf8("#DiskSelectionItem {\n"
"	background-color: rgb(39, 42, 51);\n"
"}\n"
"\n"
"QPushButton {\n"
"	background-color: rgb(39, 42, 51);\n"
"    border:0px;\n"
"	color: rgb(255, 255, 255);\n"
"	image: url(:/resources/icons/udisk.png);\n"
"	background-origin: content;\n"
"	image-position: left;\n"
"	text-align:center;\n"
"	padding-left:20px;\n"
"	padding-top:10px;\n"
"	padding-bottom:10px;\n"
"	font: 16pt \"Ubuntu\";\n"
"}\n"
"\n"
"QPushButton::pressed {\n"
"	background-color: rgb(136, 138, 133);\n"
"	color: rgb(0, 0, 0);\n"
"	image: url(:/resources/icons/udisk.png);\n"
"	background-origin: content;\n"
"	image-position: left;\n"
"	text-align:center;\n"
"	padding-left:20px;\n"
"	padding-top:10px;\n"
"	padding-bottom:10px;\n"
"	font-size: 16px;\n"
"}"));
        horizontalLayout = new QHBoxLayout(DiskSelectionItem);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        item_btn = new QPushButton(DiskSelectionItem);
        item_btn->setObjectName(QString::fromUtf8("item_btn"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(item_btn->sizePolicy().hasHeightForWidth());
        item_btn->setSizePolicy(sizePolicy1);

        horizontalLayout->addWidget(item_btn);


        retranslateUi(DiskSelectionItem);

        QMetaObject::connectSlotsByName(DiskSelectionItem);
    } // setupUi

    void retranslateUi(QWidget *DiskSelectionItem)
    {
        DiskSelectionItem->setWindowTitle(QCoreApplication::translate("DiskSelectionItem", "Form", nullptr));
        item_btn->setText(QCoreApplication::translate("DiskSelectionItem", "FDISK", nullptr));
    } // retranslateUi

};

namespace Ui {
    class DiskSelectionItem: public Ui_DiskSelectionItem {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DISKSELECTIONITEM_H
