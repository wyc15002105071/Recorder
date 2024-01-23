/********************************************************************************
** Form generated from reading UI file 'imagebrowser.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_IMAGEBROWSER_H
#define UI_IMAGEBROWSER_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ImageBrowser
{
public:
    QVBoxLayout *verticalLayout;
    QWidget *top;
    QHBoxLayout *horizontalLayout;
    QPushButton *back_btn;
    QSpacerItem *horizontalSpacer;
    QPushButton *last_btn;
    QSpacerItem *horizontalSpacer_5;
    QLabel *label;
    QSpacerItem *horizontalSpacer_6;
    QPushButton *next_btn;
    QSpacerItem *horizontalSpacer_2;
    QSpacerItem *horizontalSpacer_4;
    QWidget *container_widget;
    QHBoxLayout *horizontalLayout_2;
    QLabel *image_container;

    void setupUi(QWidget *ImageBrowser)
    {
        if (ImageBrowser->objectName().isEmpty())
            ImageBrowser->setObjectName(QString::fromUtf8("ImageBrowser"));
        ImageBrowser->resize(737, 414);
        ImageBrowser->setStyleSheet(QString::fromUtf8("#ImageBrowser {\n"
"	background-color: rgb(29, 32, 41);\n"
"}"));
        verticalLayout = new QVBoxLayout(ImageBrowser);
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        top = new QWidget(ImageBrowser);
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
        horizontalLayout->setSpacing(0);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(5, 0, 0, 0);
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

        horizontalLayout->addWidget(back_btn);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        last_btn = new QPushButton(top);
        last_btn->setObjectName(QString::fromUtf8("last_btn"));
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

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);

        horizontalSpacer_4 = new QSpacerItem(50, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_4);


        verticalLayout->addWidget(top);

        container_widget = new QWidget(ImageBrowser);
        container_widget->setObjectName(QString::fromUtf8("container_widget"));
        horizontalLayout_2 = new QHBoxLayout(container_widget);
        horizontalLayout_2->setSpacing(0);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        image_container = new QLabel(container_widget);
        image_container->setObjectName(QString::fromUtf8("image_container"));
        QSizePolicy sizePolicy2(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(image_container->sizePolicy().hasHeightForWidth());
        image_container->setSizePolicy(sizePolicy2);
        image_container->setMinimumSize(QSize(0, 0));
        image_container->setScaledContents(true);

        horizontalLayout_2->addWidget(image_container);


        verticalLayout->addWidget(container_widget);


        retranslateUi(ImageBrowser);
        QObject::connect(back_btn, SIGNAL(clicked()), ImageBrowser, SLOT(close()));
        QObject::connect(last_btn, SIGNAL(clicked()), ImageBrowser, SLOT(onLastClicked()));
        QObject::connect(next_btn, SIGNAL(clicked()), ImageBrowser, SLOT(onNextClicked()));

        QMetaObject::connectSlotsByName(ImageBrowser);
    } // setupUi

    void retranslateUi(QWidget *ImageBrowser)
    {
        ImageBrowser->setWindowTitle(QCoreApplication::translate("ImageBrowser", "Form", nullptr));
        back_btn->setText(QString());
        last_btn->setText(QString());
        label->setText(QCoreApplication::translate("ImageBrowser", "1/20", nullptr));
        next_btn->setText(QString());
        image_container->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class ImageBrowser: public Ui_ImageBrowser {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_IMAGEBROWSER_H
