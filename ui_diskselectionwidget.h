/********************************************************************************
** Form generated from reading UI file 'diskselectionwidget.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DISKSELECTIONWIDGET_H
#define UI_DISKSELECTIONWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_DiskSelectionWidget
{
public:
    QVBoxLayout *verticalLayout_2;
    QWidget *container;
    QVBoxLayout *verticalLayout;
    QLabel *label;
    QWidget *items;
    QVBoxLayout *verticalLayout_3;
    QSpacerItem *verticalSpacer;
    QPushButton *cancel_btn;

    void setupUi(QWidget *DiskSelectionWidget)
    {
        if (DiskSelectionWidget->objectName().isEmpty())
            DiskSelectionWidget->setObjectName(QString::fromUtf8("DiskSelectionWidget"));
        DiskSelectionWidget->resize(350, 142);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(DiskSelectionWidget->sizePolicy().hasHeightForWidth());
        DiskSelectionWidget->setSizePolicy(sizePolicy);
        DiskSelectionWidget->setMinimumSize(QSize(350, 0));
        DiskSelectionWidget->setMaximumSize(QSize(350, 16777215));
        DiskSelectionWidget->setStyleSheet(QString::fromUtf8(""));
        verticalLayout_2 = new QVBoxLayout(DiskSelectionWidget);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        container = new QWidget(DiskSelectionWidget);
        container->setObjectName(QString::fromUtf8("container"));
        container->setStyleSheet(QString::fromUtf8("#container {\n"
"	background-color: rgba(57, 63, 77, 1);\n"
"}\n"
"\n"
"#cancel_btn {\n"
"	background-color: rgba(39, 42, 51, 1);\n"
"   border:0px;\n"
"	color: rgb(255, 255, 255);\n"
"	font: 16pt \"Ubuntu\";\n"
"}\n"
"\n"
"#cancel_btn::pressed {\n"
"    background-color: rgb(136, 138, 133);\n"
"   border:0px;\n"
"	color: rgb(255, 255, 255);\n"
"    font: 16pt \"Ubuntu\";\n"
"}\n"
"\n"
"#DiskSelectWidget {\n"
"	background-color: rgba(29, 32, 41,1);\n"
"}\n"
"\n"
"#label {\n"
"	color: rgb(255, 255, 255);\n"
"	font: 16pt \"Ubuntu\";\n"
"}\n"
"\n"
"#item_list {\n"
"background-color: rgb(57, 63, 77);\n"
"	border:0px;\n"
"}\n"
"\n"
"#item_list::item::selected{\n"
"color:rgba(51, 51, 51, 1);\n"
"background-color: rgba(57, 63, 77, 1);\n"
"}\n"
""));
        verticalLayout = new QVBoxLayout(container);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(-1, 10, -1, 15);
        label = new QLabel(container);
        label->setObjectName(QString::fromUtf8("label"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy1);
        label->setMinimumSize(QSize(0, 30));
        label->setMaximumSize(QSize(16777215, 30));
        label->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(label);

        items = new QWidget(container);
        items->setObjectName(QString::fromUtf8("items"));
        QSizePolicy sizePolicy2(QSizePolicy::Fixed, QSizePolicy::Expanding);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(items->sizePolicy().hasHeightForWidth());
        items->setSizePolicy(sizePolicy2);
        items->setMinimumSize(QSize(300, 0));
        items->setMaximumSize(QSize(16777215, 16777215));
        verticalLayout_3 = new QVBoxLayout(items);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        verticalLayout_3->setContentsMargins(-1, -1, -1, 0);

        verticalLayout->addWidget(items, 0, Qt::AlignHCenter);

        verticalSpacer = new QSpacerItem(20, 10, QSizePolicy::Minimum, QSizePolicy::Fixed);

        verticalLayout->addItem(verticalSpacer);

        cancel_btn = new QPushButton(container);
        cancel_btn->setObjectName(QString::fromUtf8("cancel_btn"));
        QSizePolicy sizePolicy3(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(cancel_btn->sizePolicy().hasHeightForWidth());
        cancel_btn->setSizePolicy(sizePolicy3);
        cancel_btn->setMinimumSize(QSize(280, 50));
        cancel_btn->setMaximumSize(QSize(280, 50));
        cancel_btn->setFlat(false);

        verticalLayout->addWidget(cancel_btn, 0, Qt::AlignHCenter);


        verticalLayout_2->addWidget(container);


        retranslateUi(DiskSelectionWidget);
        QObject::connect(cancel_btn, SIGNAL(clicked()), DiskSelectionWidget, SLOT(close()));

        QMetaObject::connectSlotsByName(DiskSelectionWidget);
    } // setupUi

    void retranslateUi(QWidget *DiskSelectionWidget)
    {
        DiskSelectionWidget->setWindowTitle(QCoreApplication::translate("DiskSelectionWidget", "Form", nullptr));
        label->setText(QCoreApplication::translate("DiskSelectionWidget", "\346\213\267\350\264\235\345\210\260", nullptr));
        cancel_btn->setText(QCoreApplication::translate("DiskSelectionWidget", "\345\217\226\346\266\210", nullptr));
    } // retranslateUi

};

namespace Ui {
    class DiskSelectionWidget: public Ui_DiskSelectionWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DISKSELECTIONWIDGET_H
