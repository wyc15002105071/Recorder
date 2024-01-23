/********************************************************************************
** Form generated from reading UI file 'selectionicon.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SELECTIONICON_H
#define UI_SELECTIONICON_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SelectionIcon
{
public:
    QHBoxLayout *horizontalLayout;
    QPushButton *selection_btn;

    void setupUi(QWidget *SelectionIcon)
    {
        if (SelectionIcon->objectName().isEmpty())
            SelectionIcon->setObjectName(QString::fromUtf8("SelectionIcon"));
        SelectionIcon->resize(20, 20);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(SelectionIcon->sizePolicy().hasHeightForWidth());
        SelectionIcon->setSizePolicy(sizePolicy);
        SelectionIcon->setMinimumSize(QSize(20, 20));
        SelectionIcon->setMaximumSize(QSize(20, 20));
        SelectionIcon->setStyleSheet(QString::fromUtf8("#selection_btn {\n"
"   background-color:transparent;\n"
"	border-image: url(:/resources/icons/not_select.png);\n"
"}\n"
"\n"
"#selection_btn::checked {	\n"
"	border-image: url(:/resources/icons/selected.png);\n"
"}"));
        horizontalLayout = new QHBoxLayout(SelectionIcon);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        selection_btn = new QPushButton(SelectionIcon);
        selection_btn->setObjectName(QString::fromUtf8("selection_btn"));
        selection_btn->setEnabled(false);
        sizePolicy.setHeightForWidth(selection_btn->sizePolicy().hasHeightForWidth());
        selection_btn->setSizePolicy(sizePolicy);
        selection_btn->setMinimumSize(QSize(20, 20));
        selection_btn->setMaximumSize(QSize(20, 20));
        selection_btn->setCheckable(true);

        horizontalLayout->addWidget(selection_btn);


        retranslateUi(SelectionIcon);

        QMetaObject::connectSlotsByName(SelectionIcon);
    } // setupUi

    void retranslateUi(QWidget *SelectionIcon)
    {
        SelectionIcon->setWindowTitle(QCoreApplication::translate("SelectionIcon", "Form", nullptr));
        selection_btn->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class SelectionIcon: public Ui_SelectionIcon {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SELECTIONICON_H
