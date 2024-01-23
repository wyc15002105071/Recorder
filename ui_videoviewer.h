/********************************************************************************
** Form generated from reading UI file 'videoviewer.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_VIDEOVIEWER_H
#define UI_VIDEOVIEWER_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_VideoViewer
{
public:
    QVBoxLayout *verticalLayout;
    QWidget *top;
    QHBoxLayout *horizontalLayout;
    QPushButton *back_btn;
    QSpacerItem *horizontalSpacer_6;
    QPushButton *copy_btn;
    QSpacerItem *horizontalSpacer_3;
    QPushButton *copy_all_btn;
    QSpacerItem *horizontalSpacer_4;
    QPushButton *del_btn;
    QSpacerItem *horizontalSpacer_5;
    QPushButton *del_all_btn;
    QSpacerItem *horizontalSpacer;
    QPushButton *selectMode_btn;
    QSpacerItem *horizontalSpacer_7;
    QListWidget *video_list;
    QWidget *capacity_container;
    QHBoxLayout *horizontalLayout_3;
    QProgressBar *capacity;
    QWidget *extStorageWidget;
    QHBoxLayout *horizontalLayout_2;

    void setupUi(QWidget *VideoViewer)
    {
        if (VideoViewer->objectName().isEmpty())
            VideoViewer->setObjectName(QString::fromUtf8("VideoViewer"));
        VideoViewer->resize(875, 505);
        VideoViewer->setStyleSheet(QString::fromUtf8("#VideoViewer {\n"
"	background-color: rgb(29, 32, 41);\n"
"}\n"
"\n"
"\n"
"\n"
"#video_list{\n"
"   color:rgba(255, 255, 255, 1);\n"
"	border:0px;\n"
"	outline:none;\n"
"   \n"
"	background-color: rgb(29, 32, 41);\n"
"}\n"
"\n"
"#video_list::item::selected{\n"
"color:rgba(51, 51, 51, 1);\n"
"background-color: rgba(57, 63, 77, 1);\n"
"}\n"
"\n"
""));
        verticalLayout = new QVBoxLayout(VideoViewer);
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        top = new QWidget(VideoViewer);
        top->setObjectName(QString::fromUtf8("top"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(top->sizePolicy().hasHeightForWidth());
        top->setSizePolicy(sizePolicy);
        top->setMinimumSize(QSize(0, 150));
        top->setStyleSheet(QString::fromUtf8("#top {\n"
"	background-color: rgb(39, 42, 51);\n"
"}\n"
"\n"
"#back_btn{\n"
"   background-color:transparent;\n"
"   image: url(:/resources/icons/back.png);\n"
"}\n"
"\n"
"#selectMode_btn {\n"
"	image: url(:/resources/icons/select_mode.png);\n"
"	background-color: rgba(57, 63, 77, 1);\n"
"   border-radius:8px;\n"
"}\n"
"#selectMode_btn::checked {\n"
"	image: url(:/resources/icons/select_mode.png);\n"
"	background-color: rgba(24, 144, 255, 1);\n"
"   border-radius:8px;\n"
"}\n"
"\n"
"#copy_btn {\n"
"	image: url(:/resources/icons/copy.png);\n"
"}\n"
"#copy_all_btn {\n"
"	image: url(:/resources/icons/copy_all.png);\n"
"}\n"
"#del_btn {\n"
"	image: url(:/resources/icons/del.png);\n"
"}\n"
"#del_all_btn {\n"
"	image: url(:/resources/icons/del_all.png);\n"
"}\n"
"\n"
"QPushButton {\n"
"	font: 14pt \"Ubuntu\";\n"
"   border-radius:8px;\n"
"	color: rgb(225, 227, 240);\n"
"	background-color: rgba(57, 63, 77, 1);\n"
"}\n"
"\n"
"QPushButton::pressed{\n"
"   border-radius:8px;\n"
"	color: rgb(225, 227, 240);\n"
"	backgrou"
                        "nd-color: rgba(24, 144, 255, 1);\n"
"}"));
        horizontalLayout = new QHBoxLayout(top);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(5, 5, -1, 5);
        back_btn = new QPushButton(top);
        back_btn->setObjectName(QString::fromUtf8("back_btn"));
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(back_btn->sizePolicy().hasHeightForWidth());
        back_btn->setSizePolicy(sizePolicy1);
        back_btn->setMinimumSize(QSize(80, 60));
        back_btn->setMaximumSize(QSize(80, 60));
        back_btn->setFlat(true);

        horizontalLayout->addWidget(back_btn, 0, Qt::AlignLeft);

        horizontalSpacer_6 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_6);

        copy_btn = new QPushButton(top);
        copy_btn->setObjectName(QString::fromUtf8("copy_btn"));
        QSizePolicy sizePolicy2(QSizePolicy::Fixed, QSizePolicy::Expanding);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(copy_btn->sizePolicy().hasHeightForWidth());
        copy_btn->setSizePolicy(sizePolicy2);
        copy_btn->setMinimumSize(QSize(130, 50));

        horizontalLayout->addWidget(copy_btn);

        horizontalSpacer_3 = new QSpacerItem(10, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_3);

        copy_all_btn = new QPushButton(top);
        copy_all_btn->setObjectName(QString::fromUtf8("copy_all_btn"));
        QSizePolicy sizePolicy3(QSizePolicy::Fixed, QSizePolicy::MinimumExpanding);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(copy_all_btn->sizePolicy().hasHeightForWidth());
        copy_all_btn->setSizePolicy(sizePolicy3);
        copy_all_btn->setMinimumSize(QSize(130, 50));

        horizontalLayout->addWidget(copy_all_btn);

        horizontalSpacer_4 = new QSpacerItem(10, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_4);

        del_btn = new QPushButton(top);
        del_btn->setObjectName(QString::fromUtf8("del_btn"));
        sizePolicy2.setHeightForWidth(del_btn->sizePolicy().hasHeightForWidth());
        del_btn->setSizePolicy(sizePolicy2);
        del_btn->setMinimumSize(QSize(130, 50));

        horizontalLayout->addWidget(del_btn);

        horizontalSpacer_5 = new QSpacerItem(10, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_5);

        del_all_btn = new QPushButton(top);
        del_all_btn->setObjectName(QString::fromUtf8("del_all_btn"));
        sizePolicy2.setHeightForWidth(del_all_btn->sizePolicy().hasHeightForWidth());
        del_all_btn->setSizePolicy(sizePolicy2);
        del_all_btn->setMinimumSize(QSize(130, 50));

        horizontalLayout->addWidget(del_all_btn);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        selectMode_btn = new QPushButton(top);
        selectMode_btn->setObjectName(QString::fromUtf8("selectMode_btn"));
        sizePolicy1.setHeightForWidth(selectMode_btn->sizePolicy().hasHeightForWidth());
        selectMode_btn->setSizePolicy(sizePolicy1);
        selectMode_btn->setMinimumSize(QSize(60, 60));
        selectMode_btn->setCheckable(true);

        horizontalLayout->addWidget(selectMode_btn);

        horizontalSpacer_7 = new QSpacerItem(20, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_7);


        verticalLayout->addWidget(top);

        video_list = new QListWidget(VideoViewer);
        video_list->setObjectName(QString::fromUtf8("video_list"));
        video_list->setStyleSheet(QString::fromUtf8("QScrollBar {\n"
"  background: rgb(29,32,41,1);\n"
"  padding-top:18px;\n"
"  padding-bottom:18px;\n"
"  width:20px;\n"
"}\n"
"\n"
"QScrollBar::handle:vertical {\n"
"	background-color: rgba(225, 227, 240, 1);\n"
"	      border-radius:4px;\n"
"	      padding-bottom:18px;\n"
"	      padding-top:18px;\n"
"}\n"
"\n"
" QScrollBar::add-line:vertical {\n"
"      background: rgb(29,32,41,1);\n"
"	  border:1px\n"
"  }\n"
"\n"
"QScrollBar::sub-line:vertical {\n"
"      background: rgb(29,32,41,1);\n"
"\n"
"  }\n"
"\n"
"\n"
"QScrollBar::add-page:vertical\n"
"{\n"
"  background-color:rgba(29,32,41,1);\n"
"}\n"
"\n"
"QScrollBar::sub-page:vertical \n"
"{\n"
"    background-color:rgb(29,32,41,1); \n"
"}\n"
"\n"
"QScrollBar::up-arrow:vertical\n"
"{\n"
"  border-image: url(:/resources/icons/up.png);\n"
"  height:10px;\n"
"  subcontrol-position:top\n"
"}\n"
"QScrollBar::down-arrow:vertical\n"
"{\n"
"	border-image: url(:/resources/icons/down.png);\n"
"   height:10px;\n"
"   subcontrol-position:bottom\n"
"}\n"
""));
        video_list->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
        video_list->setSelectionMode(QAbstractItemView::SingleSelection);
        video_list->setIconSize(QSize(300, 300));
        video_list->setMovement(QListView::Static);
        video_list->setResizeMode(QListView::Adjust);
        video_list->setSpacing(30);
        video_list->setViewMode(QListView::IconMode);
        video_list->setBatchSize(101);
        video_list->setItemAlignment(Qt::AlignHCenter);

        verticalLayout->addWidget(video_list);

        capacity_container = new QWidget(VideoViewer);
        capacity_container->setObjectName(QString::fromUtf8("capacity_container"));
        sizePolicy.setHeightForWidth(capacity_container->sizePolicy().hasHeightForWidth());
        capacity_container->setSizePolicy(sizePolicy);
        capacity_container->setMinimumSize(QSize(0, 20));
        capacity_container->setStyleSheet(QString::fromUtf8("#capacity_container {\n"
"background-color: rgb(29, 32, 41);\n"
"}\n"
"\n"
"#capacity {\n"
"  background-color:rgba(255, 255, 255, 0.2);\n"
"  border-radius: 4px;\n"
"}\n"
"\n"
"#capacity::chunk\n"
"{\n"
"   background-color:rgba(67, 207, 124, 1);\n"
"   border-radius: 4px;\n"
"}"));
        horizontalLayout_3 = new QHBoxLayout(capacity_container);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        horizontalLayout_3->setContentsMargins(0, 0, 0, 0);
        capacity = new QProgressBar(capacity_container);
        capacity->setObjectName(QString::fromUtf8("capacity"));
        capacity->setMinimumSize(QSize(0, 10));
        capacity->setMaximumSize(QSize(16777215, 10));
        capacity->setValue(0);
        capacity->setTextVisible(false);

        horizontalLayout_3->addWidget(capacity);


        verticalLayout->addWidget(capacity_container);

        extStorageWidget = new QWidget(VideoViewer);
        extStorageWidget->setObjectName(QString::fromUtf8("extStorageWidget"));
        sizePolicy.setHeightForWidth(extStorageWidget->sizePolicy().hasHeightForWidth());
        extStorageWidget->setSizePolicy(sizePolicy);
        extStorageWidget->setMinimumSize(QSize(0, 50));
        extStorageWidget->setStyleSheet(QString::fromUtf8("#extStorageWidget {\n"
"background-color:rgba(39, 42, 51, 1);\n"
"}\n"
"\n"
"QLabel {\n"
"	color: rgb(255, 255, 255);\n"
"	font: 16pt \"Ubuntu\";\n"
"}"));
        horizontalLayout_2 = new QHBoxLayout(extStorageWidget);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(10, 0, 10, 5);

        verticalLayout->addWidget(extStorageWidget);


        retranslateUi(VideoViewer);
        QObject::connect(video_list, SIGNAL(itemClicked(QListWidgetItem*)), VideoViewer, SLOT(onItemClicked(QListWidgetItem*)));
        QObject::connect(back_btn, SIGNAL(clicked()), VideoViewer, SLOT(close()));
        QObject::connect(selectMode_btn, SIGNAL(toggled(bool)), VideoViewer, SLOT(onSelectModeToggled(bool)));
        QObject::connect(copy_all_btn, SIGNAL(clicked()), VideoViewer, SLOT(onCopyAllClicked()));
        QObject::connect(del_btn, SIGNAL(clicked()), VideoViewer, SLOT(onDelSelectClicked()));
        QObject::connect(del_all_btn, SIGNAL(clicked()), VideoViewer, SLOT(onDelAllClicked()));
        QObject::connect(copy_btn, SIGNAL(clicked()), VideoViewer, SLOT(onCopySelectedClicked()));

        QMetaObject::connectSlotsByName(VideoViewer);
    } // setupUi

    void retranslateUi(QWidget *VideoViewer)
    {
        VideoViewer->setWindowTitle(QCoreApplication::translate("VideoViewer", "Form", nullptr));
        back_btn->setText(QString());
        copy_btn->setText(QString());
        copy_all_btn->setText(QString());
        del_btn->setText(QString());
        del_all_btn->setText(QString());
        selectMode_btn->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class VideoViewer: public Ui_VideoViewer {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_VIDEOVIEWER_H
