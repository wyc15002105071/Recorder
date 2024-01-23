/********************************************************************************
** Form generated from reading UI file 'extstorageitem.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_EXTSTORAGEITEM_H
#define UI_EXTSTORAGEITEM_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QWidget>
#include "widgets/viewer/extstorageitem.h"

QT_BEGIN_NAMESPACE

class Ui_ExtStorageItem
{
public:
    QHBoxLayout *horizontalLayout;
    QWidget *container;
    QHBoxLayout *horizontalLayout_2;
    ProgressBar *capacity;

    void setupUi(QWidget *ExtStorageItem)
    {
        if (ExtStorageItem->objectName().isEmpty())
            ExtStorageItem->setObjectName(QString::fromUtf8("ExtStorageItem"));
        ExtStorageItem->resize(298, 46);
        horizontalLayout = new QHBoxLayout(ExtStorageItem);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        container = new QWidget(ExtStorageItem);
        container->setObjectName(QString::fromUtf8("container"));
        container->setStyleSheet(QString::fromUtf8("#container {\n"
"	background-color: rgba(110, 110, 110, 1);\n"
"   border-radius:5px;\n"
"}\n"
"#capacity {\n"
"  background-color:rgba(80, 80, 80, 1);\n"
"}\n"
"#capacity::chunk {\n"
"  background-color:rgba(180, 180, 180, 1);\n"
"}\n"
"\n"
"QLabel {\n"
"	font: 16pt \"Ubuntu\";\n"
"	color: rgb(255, 255, 255);\n"
"}"));
        horizontalLayout_2 = new QHBoxLayout(container);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        capacity = new ProgressBar(container);
        capacity->setObjectName(QString::fromUtf8("capacity"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(capacity->sizePolicy().hasHeightForWidth());
        capacity->setSizePolicy(sizePolicy);
        capacity->setValue(24);
        capacity->setTextVisible(false);

        horizontalLayout_2->addWidget(capacity);


        horizontalLayout->addWidget(container);


        retranslateUi(ExtStorageItem);

        QMetaObject::connectSlotsByName(ExtStorageItem);
    } // setupUi

    void retranslateUi(QWidget *ExtStorageItem)
    {
        ExtStorageItem->setWindowTitle(QCoreApplication::translate("ExtStorageItem", "Form", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ExtStorageItem: public Ui_ExtStorageItem {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_EXTSTORAGEITEM_H
