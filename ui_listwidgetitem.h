/********************************************************************************
** Form generated from reading UI file 'listwidgetitem.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LISTWIDGETITEM_H
#define UI_LISTWIDGETITEM_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "widgets/viewer/listwidgetitem.h"

QT_BEGIN_NAMESPACE

class Ui_ListWidgetItem
{
public:
    QVBoxLayout *verticalLayout;
    Label *icon;
    QLabel *file_name;

    void setupUi(QWidget *ListWidgetItem)
    {
        if (ListWidgetItem->objectName().isEmpty())
            ListWidgetItem->setObjectName(QString::fromUtf8("ListWidgetItem"));
        ListWidgetItem->resize(281, 154);
        ListWidgetItem->setStyleSheet(QString::fromUtf8("#file_name {\n"
"	color: rgb(255, 255, 255);\n"
"	font: 10pt \"Ubuntu\";\n"
"}"));
        verticalLayout = new QVBoxLayout(ListWidgetItem);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        icon = new Label(ListWidgetItem);
        icon->setObjectName(QString::fromUtf8("icon"));

        verticalLayout->addWidget(icon);

        file_name = new QLabel(ListWidgetItem);
        file_name->setObjectName(QString::fromUtf8("file_name"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(file_name->sizePolicy().hasHeightForWidth());
        file_name->setSizePolicy(sizePolicy);
        file_name->setMinimumSize(QSize(0, 20));
        file_name->setMaximumSize(QSize(16777215, 20));
        file_name->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(file_name);


        retranslateUi(ListWidgetItem);

        QMetaObject::connectSlotsByName(ListWidgetItem);
    } // setupUi

    void retranslateUi(QWidget *ListWidgetItem)
    {
        ListWidgetItem->setWindowTitle(QCoreApplication::translate("ListWidgetItem", "Form", nullptr));
        icon->setText(QString());
        file_name->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class ListWidgetItem: public Ui_ListWidgetItem {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LISTWIDGETITEM_H
