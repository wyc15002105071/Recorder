/********************************************************************************
** Form generated from reading UI file 'menuwidget.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MENUWIDGET_H
#define UI_MENUWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MenuWidget
{
public:
    QHBoxLayout *horizontalLayout_2;
    QWidget *menu_widget;
    QHBoxLayout *horizontalLayout;
    QPushButton *capture_btn;
    QPushButton *record_btn;
    QPushButton *push_btn;
    QPushButton *picture_btn;
    QPushButton *video_btn;

    void setupUi(QWidget *MenuWidget)
    {
        if (MenuWidget->objectName().isEmpty())
            MenuWidget->setObjectName(QString::fromUtf8("MenuWidget"));
        MenuWidget->resize(803, 233);
        MenuWidget->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"background-color: qlineargradient(spread:pad, x1: 0, y1: 0, x2: 0, y2: 1, stop:0\n"
"rgba(43, 202, 255, 1), stop:1.0 rgba(0, 90, 245, 1));\n"
"border-radius: 40px;\n"
"}\n"
"\n"
"#capture_btn {\n"
"	image: url(:/resources/icons/capture.png);\n"
"}\n"
"\n"
"#record_btn {\n"
"	image: url(:/resources/icons/record.png);\n"
"}\n"
"\n"
"#push_btn {\n"
"	image: url(:/resources/icons/share.png);\n"
"}\n"
"\n"
"#video_btn {\n"
"	image: url(:/resources/icons/video.png);\n"
"}\n"
"\n"
"#picture_btn {\n"
"	image: url(:/resources/icons/picture.png);\n"
"}\n"
"\n"
"#menu_widget{\n"
"	background-color: rgba(0, 0, 0, 0.4);\n"
"}\n"
""));
        horizontalLayout_2 = new QHBoxLayout(MenuWidget);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        menu_widget = new QWidget(MenuWidget);
        menu_widget->setObjectName(QString::fromUtf8("menu_widget"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(menu_widget->sizePolicy().hasHeightForWidth());
        menu_widget->setSizePolicy(sizePolicy);
        menu_widget->setMinimumSize(QSize(0, 200));
        menu_widget->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"background-color: qlineargradient(spread:pad, x1: 0, y1: 0, x2: 0, y2: 1, stop:0\n"
"rgba(43, 202, 255, 1), stop:1.0 rgba(0, 90, 245, 1));\n"
"border-radius: 40px;\n"
"}\n"
"\n"
"#capture_btn {\n"
"	image: url(:/resources/icons/capture.png);\n"
"}\n"
"\n"
"#record_btn {\n"
"	image: url(:/resources/icons/record.png);\n"
"}\n"
"\n"
"#share_btn {\n"
"	image: url(:/resources/icons/share.png);\n"
"}\n"
"\n"
"#video_btn {\n"
"	image: url(:/resources/icons/video.png);\n"
"}\n"
"\n"
"#picture_btn {\n"
"	image: url(:/resources/icons/picture.png);\n"
"}"));
        horizontalLayout = new QHBoxLayout(menu_widget);
        horizontalLayout->setSpacing(15);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(15, -1, 15, -1);
        capture_btn = new QPushButton(menu_widget);
        capture_btn->setObjectName(QString::fromUtf8("capture_btn"));
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(capture_btn->sizePolicy().hasHeightForWidth());
        capture_btn->setSizePolicy(sizePolicy1);
        capture_btn->setMinimumSize(QSize(140, 140));

        horizontalLayout->addWidget(capture_btn);

        record_btn = new QPushButton(menu_widget);
        record_btn->setObjectName(QString::fromUtf8("record_btn"));
        sizePolicy1.setHeightForWidth(record_btn->sizePolicy().hasHeightForWidth());
        record_btn->setSizePolicy(sizePolicy1);
        record_btn->setMinimumSize(QSize(140, 140));

        horizontalLayout->addWidget(record_btn);

        push_btn = new QPushButton(menu_widget);
        push_btn->setObjectName(QString::fromUtf8("push_btn"));
        sizePolicy1.setHeightForWidth(push_btn->sizePolicy().hasHeightForWidth());
        push_btn->setSizePolicy(sizePolicy1);
        push_btn->setMinimumSize(QSize(140, 140));

        horizontalLayout->addWidget(push_btn);

        picture_btn = new QPushButton(menu_widget);
        picture_btn->setObjectName(QString::fromUtf8("picture_btn"));
        sizePolicy1.setHeightForWidth(picture_btn->sizePolicy().hasHeightForWidth());
        picture_btn->setSizePolicy(sizePolicy1);
        picture_btn->setMinimumSize(QSize(140, 140));

        horizontalLayout->addWidget(picture_btn);

        video_btn = new QPushButton(menu_widget);
        video_btn->setObjectName(QString::fromUtf8("video_btn"));
        sizePolicy1.setHeightForWidth(video_btn->sizePolicy().hasHeightForWidth());
        video_btn->setSizePolicy(sizePolicy1);
        video_btn->setMinimumSize(QSize(140, 140));

        horizontalLayout->addWidget(video_btn);


        horizontalLayout_2->addWidget(menu_widget);


        retranslateUi(MenuWidget);
        QObject::connect(capture_btn, SIGNAL(clicked()), MenuWidget, SLOT(onCaptureClicked()));
        QObject::connect(record_btn, SIGNAL(clicked()), MenuWidget, SLOT(onRecordClicked()));
        QObject::connect(push_btn, SIGNAL(clicked()), MenuWidget, SLOT(onPushClicked()));
        QObject::connect(picture_btn, SIGNAL(clicked()), MenuWidget, SLOT(onPictureFileClicked()));
        QObject::connect(video_btn, SIGNAL(clicked()), MenuWidget, SLOT(onVideoFileClicked()));

        QMetaObject::connectSlotsByName(MenuWidget);
    } // setupUi

    void retranslateUi(QWidget *MenuWidget)
    {
        MenuWidget->setWindowTitle(QCoreApplication::translate("MenuWidget", "Form", nullptr));
        capture_btn->setText(QString());
        record_btn->setText(QString());
        push_btn->setText(QString());
        picture_btn->setText(QString());
        video_btn->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class MenuWidget: public Ui_MenuWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MENUWIDGET_H
