/********************************************************************************
** Form generated from reading UI file 'recordwidget.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_RECORDWIDGET_H
#define UI_RECORDWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "widgets/record_timer_widget.h"

QT_BEGIN_NAMESPACE

class Ui_RecordWidget
{
public:
    QVBoxLayout *verticalLayout;
    QGridLayout *gridLayout;
    QSpacerItem *verticalSpacer;
    QLabel *bottom_left;
    QSpacerItem *verticalSpacer_4;
    QLabel *rect;
    QSpacerItem *horizontalSpacer;
    QLabel *top_right;
    QLabel *bottom_right;
    QSpacerItem *verticalSpacer_2;
    QSpacerItem *horizontalSpacer_2;
    QSpacerItem *verticalSpacer_3;
    QLabel *top_left;
    QWidget *widget;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer_3;
    QPushButton *record_btn;
    RecordTimerWidget *record_timer_widget;
    QSpacerItem *horizontalSpacer_4;

    void setupUi(QWidget *RecordWidget)
    {
        if (RecordWidget->objectName().isEmpty())
            RecordWidget->setObjectName(QString::fromUtf8("RecordWidget"));
        RecordWidget->resize(746, 554);
        RecordWidget->setStyleSheet(QString::fromUtf8("QLabel {\n"
"	font: 20pt \"Ubuntu\";\n"
"}\n"
"#top_left {\n"
"	border-image: url(:/resources/icons/top_left.png);\n"
"}\n"
"#top_right {\n"
"	border-image: url(:/resources/icons/top_right.png);\n"
"}\n"
"#bottom_left {\n"
"	border-image: url(:/resources/icons/bottom_left.png);\n"
"}\n"
"#bottom_right {\n"
"	border-image: url(:/resources/icons/bottom_right.png);\n"
"}\n"
"#rect {\n"
"	border-image: url(:/resources/icons/rect.png);\n"
"}\n"
"#record_btn {\n"
"	\n"
"	image: url(:/resources/icons/record_stop.png);\n"
"border:2px groove black;\n"
"border-radius:30px;\n"
"}"));
        verticalLayout = new QVBoxLayout(RecordWidget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setHorizontalSpacing(6);
        gridLayout->setContentsMargins(10, 10, 10, 10);
        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(verticalSpacer, 1, 0, 1, 1);

        bottom_left = new QLabel(RecordWidget);
        bottom_left->setObjectName(QString::fromUtf8("bottom_left"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(bottom_left->sizePolicy().hasHeightForWidth());
        bottom_left->setSizePolicy(sizePolicy);
        bottom_left->setMinimumSize(QSize(113, 113));

        gridLayout->addWidget(bottom_left, 4, 0, 1, 1, Qt::AlignLeft);

        verticalSpacer_4 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(verticalSpacer_4, 3, 5, 1, 1);

        rect = new QLabel(RecordWidget);
        rect->setObjectName(QString::fromUtf8("rect"));
        sizePolicy.setHeightForWidth(rect->sizePolicy().hasHeightForWidth());
        rect->setSizePolicy(sizePolicy);
        rect->setMinimumSize(QSize(121, 121));

        gridLayout->addWidget(rect, 2, 3, 1, 1);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer, 0, 1, 1, 1);

        top_right = new QLabel(RecordWidget);
        top_right->setObjectName(QString::fromUtf8("top_right"));
        sizePolicy.setHeightForWidth(top_right->sizePolicy().hasHeightForWidth());
        top_right->setSizePolicy(sizePolicy);
        top_right->setMinimumSize(QSize(113, 113));
        top_right->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(top_right, 0, 5, 1, 1, Qt::AlignHCenter);

        bottom_right = new QLabel(RecordWidget);
        bottom_right->setObjectName(QString::fromUtf8("bottom_right"));
        sizePolicy.setHeightForWidth(bottom_right->sizePolicy().hasHeightForWidth());
        bottom_right->setSizePolicy(sizePolicy);
        bottom_right->setMinimumSize(QSize(113, 113));

        gridLayout->addWidget(bottom_right, 4, 5, 1, 1, Qt::AlignRight);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(verticalSpacer_2, 1, 5, 1, 1);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_2, 0, 4, 1, 1);

        verticalSpacer_3 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(verticalSpacer_3, 3, 0, 1, 1);

        top_left = new QLabel(RecordWidget);
        top_left->setObjectName(QString::fromUtf8("top_left"));
        sizePolicy.setHeightForWidth(top_left->sizePolicy().hasHeightForWidth());
        top_left->setSizePolicy(sizePolicy);
        top_left->setMinimumSize(QSize(113, 113));
        top_left->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(top_left, 0, 0, 1, 1, Qt::AlignLeft);

        widget = new QWidget(RecordWidget);
        widget->setObjectName(QString::fromUtf8("widget"));
        horizontalLayout = new QHBoxLayout(widget);
        horizontalLayout->setSpacing(5);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_3);

        record_btn = new QPushButton(widget);
        record_btn->setObjectName(QString::fromUtf8("record_btn"));
        sizePolicy.setHeightForWidth(record_btn->sizePolicy().hasHeightForWidth());
        record_btn->setSizePolicy(sizePolicy);
        record_btn->setMinimumSize(QSize(60, 60));
        record_btn->setCheckable(true);

        horizontalLayout->addWidget(record_btn);

        record_timer_widget = new RecordTimerWidget(widget);
        record_timer_widget->setObjectName(QString::fromUtf8("record_timer_widget"));
        sizePolicy.setHeightForWidth(record_timer_widget->sizePolicy().hasHeightForWidth());
        record_timer_widget->setSizePolicy(sizePolicy);
        record_timer_widget->setMinimumSize(QSize(261, 47));

        horizontalLayout->addWidget(record_timer_widget);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_4);


        gridLayout->addWidget(widget, 4, 1, 1, 4);


        verticalLayout->addLayout(gridLayout);


        retranslateUi(RecordWidget);
        QObject::connect(record_btn, SIGNAL(toggled(bool)), RecordWidget, SLOT(onRecordBtnToggled(bool)));

        QMetaObject::connectSlotsByName(RecordWidget);
    } // setupUi

    void retranslateUi(QWidget *RecordWidget)
    {
        RecordWidget->setWindowTitle(QCoreApplication::translate("RecordWidget", "Form", nullptr));
        bottom_left->setText(QString());
        rect->setText(QString());
        top_right->setText(QCoreApplication::translate("RecordWidget", "REC", nullptr));
        bottom_right->setText(QString());
        top_left->setText(QString());
        record_btn->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class RecordWidget: public Ui_RecordWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_RECORDWIDGET_H
