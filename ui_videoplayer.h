/********************************************************************************
** Form generated from reading UI file 'videoplayer.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_VIDEOPLAYER_H
#define UI_VIDEOPLAYER_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_VideoPlayer
{
public:
    QVBoxLayout *verticalLayout;
    QWidget *top;
    QHBoxLayout *horizontalLayout;
    QPushButton *back_btn;
    QSpacerItem *horizontalSpacer_3;
    QPushButton *last_btn;
    QSpacerItem *horizontalSpacer_5;
    QLabel *label;
    QSpacerItem *horizontalSpacer_6;
    QPushButton *next_btn;
    QSpacerItem *horizontalSpacer;
    QSpacerItem *horizontalSpacer_4;
    QWidget *video_widget;
    QHBoxLayout *horizontalLayout_2;
    QWidget *bottom;
    QHBoxLayout *horizontalLayout_3;
    QSpacerItem *horizontalSpacer_11;
    QPushButton *play_btn;
    QSpacerItem *horizontalSpacer_7;
    QLabel *position;
    QSpacerItem *horizontalSpacer_8;
    QSlider *pos_slider;
    QSpacerItem *horizontalSpacer_9;
    QLabel *duration;
    QSpacerItem *horizontalSpacer_10;

    void setupUi(QWidget *VideoPlayer)
    {
        if (VideoPlayer->objectName().isEmpty())
            VideoPlayer->setObjectName(QString::fromUtf8("VideoPlayer"));
        VideoPlayer->resize(609, 400);
        VideoPlayer->setStyleSheet(QString::fromUtf8("#VideoPlayer {\n"
"background-color: rgb(29, 32, 41);\n"
"}\n"
"\n"
"\n"
""));
        verticalLayout = new QVBoxLayout(VideoPlayer);
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        top = new QWidget(VideoPlayer);
        top->setObjectName(QString::fromUtf8("top"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(top->sizePolicy().hasHeightForWidth());
        top->setSizePolicy(sizePolicy);
        top->setMinimumSize(QSize(0, 80));
        top->setMaximumSize(QSize(16777215, 80));
        top->setStyleSheet(QString::fromUtf8("#top {\n"
"background-color: rgb(39, 42, 51);\n"
"}\n"
"\n"
"#back_btn {\n"
"   background-color: rgb(39, 42, 51);\n"
"	image: url(:/resources/icons/back.png);\n"
"}\n"
"\n"
"#last_btn {\n"
"	border-image: url(:/resources/icons/last.png);\n"
"}\n"
"#next_btn {\n"
"	border-image: url(:/resources/icons/next.png);\n"
"}\n"
"#label {\n"
"	color: rgb(255, 255, 255);\n"
"  \n"
"	font: 18pt \"Ubuntu\";\n"
"}"));
        horizontalLayout = new QHBoxLayout(top);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(5, -1, -1, -1);
        back_btn = new QPushButton(top);
        back_btn->setObjectName(QString::fromUtf8("back_btn"));
        back_btn->setMinimumSize(QSize(80, 60));
        back_btn->setMaximumSize(QSize(80, 60));
        back_btn->setFlat(true);

        horizontalLayout->addWidget(back_btn);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_3);

        last_btn = new QPushButton(top);
        last_btn->setObjectName(QString::fromUtf8("last_btn"));
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(last_btn->sizePolicy().hasHeightForWidth());
        last_btn->setSizePolicy(sizePolicy1);
        last_btn->setMinimumSize(QSize(25, 45));
        last_btn->setMaximumSize(QSize(25, 45));

        horizontalLayout->addWidget(last_btn);

        horizontalSpacer_5 = new QSpacerItem(10, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_5);

        label = new QLabel(top);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout->addWidget(label);

        horizontalSpacer_6 = new QSpacerItem(10, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_6);

        next_btn = new QPushButton(top);
        next_btn->setObjectName(QString::fromUtf8("next_btn"));
        sizePolicy1.setHeightForWidth(next_btn->sizePolicy().hasHeightForWidth());
        next_btn->setSizePolicy(sizePolicy1);
        next_btn->setMinimumSize(QSize(25, 45));
        next_btn->setMaximumSize(QSize(25, 45));

        horizontalLayout->addWidget(next_btn);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        horizontalSpacer_4 = new QSpacerItem(48, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_4);


        verticalLayout->addWidget(top);

        video_widget = new QWidget(VideoPlayer);
        video_widget->setObjectName(QString::fromUtf8("video_widget"));
        video_widget->setStyleSheet(QString::fromUtf8(""));
        horizontalLayout_2 = new QHBoxLayout(video_widget);
        horizontalLayout_2->setSpacing(0);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);

        verticalLayout->addWidget(video_widget);

        bottom = new QWidget(VideoPlayer);
        bottom->setObjectName(QString::fromUtf8("bottom"));
        sizePolicy.setHeightForWidth(bottom->sizePolicy().hasHeightForWidth());
        bottom->setSizePolicy(sizePolicy);
        bottom->setMinimumSize(QSize(0, 60));
        bottom->setMaximumSize(QSize(16777215, 60));
        bottom->setStyleSheet(QString::fromUtf8("#bottom {\n"
"	background-color: rgb(0, 0, 0);\n"
"}\n"
"\n"
"QLabel {\n"
"	color: rgb(255, 255, 255);\n"
"	font: 14pt \"Ubuntu\";\n"
"}\n"
"\n"
"#play_btn {\n"
"	border-image: url(:/resources/icons/play.png);\n"
"}\n"
"\n"
"#play_btn::checked{\n"
"	border-image: url(:/resources/icons/pause.png);\n"
"}\n"
"\n"
"QSlider::sub-page:horizontal {\n"
"    background: rgba(24, 144, 255, 1);\n"
"    border-radius: 2px;\n"
"    margin-top:8px;\n"
"    margin-bottom:8px;\n"
"}\n"
"QSlider::add-page:horizontal {\n"
"    background: rgb(255,255, 255);\n"
"    border-radius: 2px;\n"
"    margin-top:9px;\n"
"    margin-bottom:9px;\n"
"}\n"
" \n"
"QSlider::handle:horizontal {\n"
"    background: rgb(193,204,208);\n"
"    width: 5px;\n"
"    border: 1px solid rgb(193,204,208);\n"
"    border-radius: 5px; \n"
"    margin-top:6px;\n"
"    margin-bottom:6px;\n"
"}\n"
" \n"
"QSlider::handle:horizontal:hover {\n"
"    background: rgb(193,204,208);\n"
"    width: 10px;\n"
"    border: 1px solid rgb(193,204,208);       \n"
"    bord"
                        "er-radius: 5px; \n"
"    margin-top:4px;\n"
"    margin-bottom:4px;\n"
"}	"));
        horizontalLayout_3 = new QHBoxLayout(bottom);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        horizontalSpacer_11 = new QSpacerItem(10, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_11);

        play_btn = new QPushButton(bottom);
        play_btn->setObjectName(QString::fromUtf8("play_btn"));
        sizePolicy1.setHeightForWidth(play_btn->sizePolicy().hasHeightForWidth());
        play_btn->setSizePolicy(sizePolicy1);
        play_btn->setMinimumSize(QSize(20, 30));
        play_btn->setMaximumSize(QSize(20, 30));
        play_btn->setCheckable(true);

        horizontalLayout_3->addWidget(play_btn);

        horizontalSpacer_7 = new QSpacerItem(10, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_7);

        position = new QLabel(bottom);
        position->setObjectName(QString::fromUtf8("position"));

        horizontalLayout_3->addWidget(position);

        horizontalSpacer_8 = new QSpacerItem(10, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_8);

        pos_slider = new QSlider(bottom);
        pos_slider->setObjectName(QString::fromUtf8("pos_slider"));
        QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(pos_slider->sizePolicy().hasHeightForWidth());
        pos_slider->setSizePolicy(sizePolicy2);
        pos_slider->setMinimumSize(QSize(0, 10));
        pos_slider->setMaximumSize(QSize(16777215, 20));
        pos_slider->setStyleSheet(QString::fromUtf8(""));
        pos_slider->setMaximum(100);
        pos_slider->setValue(0);
        pos_slider->setOrientation(Qt::Horizontal);

        horizontalLayout_3->addWidget(pos_slider);

        horizontalSpacer_9 = new QSpacerItem(10, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_9);

        duration = new QLabel(bottom);
        duration->setObjectName(QString::fromUtf8("duration"));

        horizontalLayout_3->addWidget(duration);

        horizontalSpacer_10 = new QSpacerItem(10, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_10);


        verticalLayout->addWidget(bottom);


        retranslateUi(VideoPlayer);
        QObject::connect(last_btn, SIGNAL(clicked()), VideoPlayer, SLOT(onLastClicked()));
        QObject::connect(next_btn, SIGNAL(clicked()), VideoPlayer, SLOT(onNextClicked()));
        QObject::connect(play_btn, SIGNAL(toggled(bool)), VideoPlayer, SLOT(onPlayChecked(bool)));
        QObject::connect(back_btn, SIGNAL(clicked()), VideoPlayer, SLOT(onBackClicked()));

        QMetaObject::connectSlotsByName(VideoPlayer);
    } // setupUi

    void retranslateUi(QWidget *VideoPlayer)
    {
        VideoPlayer->setWindowTitle(QCoreApplication::translate("VideoPlayer", "Form", nullptr));
        back_btn->setText(QString());
        last_btn->setText(QString());
        label->setText(QCoreApplication::translate("VideoPlayer", "1/20", nullptr));
        next_btn->setText(QString());
        play_btn->setText(QString());
        position->setText(QCoreApplication::translate("VideoPlayer", "00:00:00", nullptr));
        duration->setText(QCoreApplication::translate("VideoPlayer", "10:10:10", nullptr));
    } // retranslateUi

};

namespace Ui {
    class VideoPlayer: public Ui_VideoPlayer {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_VIDEOPLAYER_H
