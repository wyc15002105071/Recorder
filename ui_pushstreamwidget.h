/********************************************************************************
** Form generated from reading UI file 'pushstreamwidget.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PUSHSTREAMWIDGET_H
#define UI_PUSHSTREAMWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_PushStreamWidget
{
public:
    QHBoxLayout *horizontalLayout_2;
    QWidget *widget;
    QVBoxLayout *verticalLayout;
    QWidget *ctrl_widget;
    QHBoxLayout *horizontalLayout;
    QPushButton *push;
    QPushButton *back;
    QLabel *url;

    void setupUi(QWidget *PushStreamWidget)
    {
        if (PushStreamWidget->objectName().isEmpty())
            PushStreamWidget->setObjectName(QString::fromUtf8("PushStreamWidget"));
        PushStreamWidget->resize(605, 158);
        PushStreamWidget->setStyleSheet(QString::fromUtf8(""));
        horizontalLayout_2 = new QHBoxLayout(PushStreamWidget);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(-1, 0, -1, 0);
        widget = new QWidget(PushStreamWidget);
        widget->setObjectName(QString::fromUtf8("widget"));
        widget->setStyleSheet(QString::fromUtf8(""));
        verticalLayout = new QVBoxLayout(widget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        ctrl_widget = new QWidget(widget);
        ctrl_widget->setObjectName(QString::fromUtf8("ctrl_widget"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(ctrl_widget->sizePolicy().hasHeightForWidth());
        ctrl_widget->setSizePolicy(sizePolicy);
        ctrl_widget->setMinimumSize(QSize(340, 0));
        ctrl_widget->setMaximumSize(QSize(340, 16777215));
        ctrl_widget->setStyleSheet(QString::fromUtf8("#ctrl_widget {\n"
"	background-color: rgba(0, 0, 0, 0.4);\n"
"    border-radius:40px;\n"
"}\n"
"\n"
"#back {\n"
"	border-image: url(:/resources/icons/push_exit.png);\n"
"}\n"
"\n"
"#push {\n"
"	border-image: url(:/resources/icons/push.png);\n"
"}\n"
"\n"
"#push::checked{\n"
"	border-image: url(:/resources/icons/push_toggled.png);\n"
"}"));
        horizontalLayout = new QHBoxLayout(ctrl_widget);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        push = new QPushButton(ctrl_widget);
        push->setObjectName(QString::fromUtf8("push"));
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(push->sizePolicy().hasHeightForWidth());
        push->setSizePolicy(sizePolicy1);
        push->setMinimumSize(QSize(80, 80));
        push->setMaximumSize(QSize(80, 80));
        push->setCheckable(true);

        horizontalLayout->addWidget(push);

        back = new QPushButton(ctrl_widget);
        back->setObjectName(QString::fromUtf8("back"));
        sizePolicy1.setHeightForWidth(back->sizePolicy().hasHeightForWidth());
        back->setSizePolicy(sizePolicy1);
        back->setMinimumSize(QSize(80, 80));
        back->setMaximumSize(QSize(80, 80));

        horizontalLayout->addWidget(back);


        verticalLayout->addWidget(ctrl_widget, 0, Qt::AlignHCenter);

        url = new QLabel(widget);
        url->setObjectName(QString::fromUtf8("url"));
        url->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"font: 16pt \"Ubuntu\";"));
        url->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(url);


        horizontalLayout_2->addWidget(widget);


        retranslateUi(PushStreamWidget);
        QObject::connect(back, SIGNAL(clicked()), PushStreamWidget, SLOT(close()));
        QObject::connect(push, SIGNAL(toggled(bool)), PushStreamWidget, SLOT(onPushToggled(bool)));

        QMetaObject::connectSlotsByName(PushStreamWidget);
    } // setupUi

    void retranslateUi(QWidget *PushStreamWidget)
    {
        PushStreamWidget->setWindowTitle(QCoreApplication::translate("PushStreamWidget", "Form", nullptr));
        push->setText(QString());
        back->setText(QString());
        url->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class PushStreamWidget: public Ui_PushStreamWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PUSHSTREAMWIDGET_H
