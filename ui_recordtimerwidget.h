/********************************************************************************
** Form generated from reading UI file 'recordtimerwidget.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_RECORDTIMERWIDGET_H
#define UI_RECORDTIMERWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_RecordTimerWidget
{
public:
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QLabel *record_time_str;
    QSpacerItem *horizontalSpacer_2;

    void setupUi(QWidget *RecordTimerWidget)
    {
        if (RecordTimerWidget->objectName().isEmpty())
            RecordTimerWidget->setObjectName(QString::fromUtf8("RecordTimerWidget"));
        RecordTimerWidget->resize(256, 63);
        RecordTimerWidget->setStyleSheet(QString::fromUtf8("#RecordTimerWidget {\n"
"\n"
"}\n"
"\n"
"#record_time_str {\n"
"	color: rgb(0, 0, 0);\n"
"  \n"
"	font: 20pt \"Ubuntu\";\n"
"}\n"
"\n"
"#recording_flag {\n"
"	border-image: url(:/resources/icons/not_push.png);\n"
"}\n"
"\n"
"#recording_flag::checked{\n"
"	border-image: url(:/resources/icons/recording.png);\n"
"}"));
        horizontalLayout = new QHBoxLayout(RecordTimerWidget);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(-1, -1, 9, -1);
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        record_time_str = new QLabel(RecordTimerWidget);
        record_time_str->setObjectName(QString::fromUtf8("record_time_str"));

        horizontalLayout->addWidget(record_time_str, 0, Qt::AlignHCenter|Qt::AlignVCenter);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);


        retranslateUi(RecordTimerWidget);

        QMetaObject::connectSlotsByName(RecordTimerWidget);
    } // setupUi

    void retranslateUi(QWidget *RecordTimerWidget)
    {
        RecordTimerWidget->setWindowTitle(QCoreApplication::translate("RecordTimerWidget", "Form", nullptr));
        record_time_str->setText(QCoreApplication::translate("RecordTimerWidget", "00:00:00", nullptr));
    } // retranslateUi

};

namespace Ui {
    class RecordTimerWidget: public Ui_RecordTimerWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_RECORDTIMERWIDGET_H
