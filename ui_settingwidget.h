/********************************************************************************
** Form generated from reading UI file 'settingwidget.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SETTINGWIDGET_H
#define UI_SETTINGWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SettingWidget
{
public:
    QGridLayout *gridLayout;
    QWidget *settings_view;
    QVBoxLayout *verticalLayout;
    QGroupBox *groupBox;
    QHBoxLayout *horizontalLayout;
    QRadioButton *radioButton;
    QRadioButton *radioButton_2;
    QRadioButton *radioButton_3;
    QGroupBox *groupBox_2;
    QHBoxLayout *horizontalLayout_2;
    QRadioButton *radioButton_4;
    QRadioButton *radioButton_5;
    QRadioButton *radioButton_6;
    QGroupBox *groupBox_3;
    QHBoxLayout *horizontalLayout_3;
    QRadioButton *radioButton_7;
    QRadioButton *radioButton_8;
    QRadioButton *radioButton_9;
    QHBoxLayout *horizontalLayout_4;
    QSpacerItem *horizontalSpacer;
    QPushButton *ensure_btn;

    void setupUi(QWidget *SettingWidget)
    {
        if (SettingWidget->objectName().isEmpty())
            SettingWidget->setObjectName(QString::fromUtf8("SettingWidget"));
        SettingWidget->resize(639, 491);
        SettingWidget->setStyleSheet(QString::fromUtf8("#SettingWidget{\n"
"	background-color: rgba(155, 155, 155, 0.3);\n"
"}\n"
"\n"
"#settings_view {\n"
"	background-color: rgb(255, 255, 255);\n"
"}\n"
"\n"
"QGroupBox{\n"
"border: none;\n"
"}\n"
"\n"
"QGroupBox::title\n"
"{\n"
"subcontrol-origin:margin;\n"
"padding:1px;\n"
"}"));
        gridLayout = new QGridLayout(SettingWidget);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setContentsMargins(100, 100, 100, 100);
        settings_view = new QWidget(SettingWidget);
        settings_view->setObjectName(QString::fromUtf8("settings_view"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(settings_view->sizePolicy().hasHeightForWidth());
        settings_view->setSizePolicy(sizePolicy);
        settings_view->setMinimumSize(QSize(400, 300));
        verticalLayout = new QVBoxLayout(settings_view);
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(-1, -1, -1, 0);
        groupBox = new QGroupBox(settings_view);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(groupBox->sizePolicy().hasHeightForWidth());
        groupBox->setSizePolicy(sizePolicy1);
        groupBox->setMinimumSize(QSize(0, 80));
        horizontalLayout = new QHBoxLayout(groupBox);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        radioButton = new QRadioButton(groupBox);
        radioButton->setObjectName(QString::fromUtf8("radioButton"));

        horizontalLayout->addWidget(radioButton, 0, Qt::AlignHCenter);

        radioButton_2 = new QRadioButton(groupBox);
        radioButton_2->setObjectName(QString::fromUtf8("radioButton_2"));

        horizontalLayout->addWidget(radioButton_2, 0, Qt::AlignHCenter);

        radioButton_3 = new QRadioButton(groupBox);
        radioButton_3->setObjectName(QString::fromUtf8("radioButton_3"));

        horizontalLayout->addWidget(radioButton_3, 0, Qt::AlignHCenter);


        verticalLayout->addWidget(groupBox);

        groupBox_2 = new QGroupBox(settings_view);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        sizePolicy1.setHeightForWidth(groupBox_2->sizePolicy().hasHeightForWidth());
        groupBox_2->setSizePolicy(sizePolicy1);
        groupBox_2->setMinimumSize(QSize(0, 80));
        horizontalLayout_2 = new QHBoxLayout(groupBox_2);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        radioButton_4 = new QRadioButton(groupBox_2);
        radioButton_4->setObjectName(QString::fromUtf8("radioButton_4"));

        horizontalLayout_2->addWidget(radioButton_4, 0, Qt::AlignHCenter);

        radioButton_5 = new QRadioButton(groupBox_2);
        radioButton_5->setObjectName(QString::fromUtf8("radioButton_5"));

        horizontalLayout_2->addWidget(radioButton_5, 0, Qt::AlignHCenter);

        radioButton_6 = new QRadioButton(groupBox_2);
        radioButton_6->setObjectName(QString::fromUtf8("radioButton_6"));

        horizontalLayout_2->addWidget(radioButton_6, 0, Qt::AlignHCenter);


        verticalLayout->addWidget(groupBox_2);

        groupBox_3 = new QGroupBox(settings_view);
        groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
        sizePolicy1.setHeightForWidth(groupBox_3->sizePolicy().hasHeightForWidth());
        groupBox_3->setSizePolicy(sizePolicy1);
        groupBox_3->setMinimumSize(QSize(0, 80));
        horizontalLayout_3 = new QHBoxLayout(groupBox_3);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        radioButton_7 = new QRadioButton(groupBox_3);
        radioButton_7->setObjectName(QString::fromUtf8("radioButton_7"));

        horizontalLayout_3->addWidget(radioButton_7, 0, Qt::AlignHCenter);

        radioButton_8 = new QRadioButton(groupBox_3);
        radioButton_8->setObjectName(QString::fromUtf8("radioButton_8"));

        horizontalLayout_3->addWidget(radioButton_8, 0, Qt::AlignHCenter);

        radioButton_9 = new QRadioButton(groupBox_3);
        radioButton_9->setObjectName(QString::fromUtf8("radioButton_9"));

        horizontalLayout_3->addWidget(radioButton_9, 0, Qt::AlignHCenter);


        verticalLayout->addWidget(groupBox_3);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer);

        ensure_btn = new QPushButton(settings_view);
        ensure_btn->setObjectName(QString::fromUtf8("ensure_btn"));
        QSizePolicy sizePolicy2(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(ensure_btn->sizePolicy().hasHeightForWidth());
        ensure_btn->setSizePolicy(sizePolicy2);

        horizontalLayout_4->addWidget(ensure_btn);


        verticalLayout->addLayout(horizontalLayout_4);


        gridLayout->addWidget(settings_view, 0, 1, 1, 1);


        retranslateUi(SettingWidget);
        QObject::connect(ensure_btn, SIGNAL(clicked()), SettingWidget, SLOT(close()));

        QMetaObject::connectSlotsByName(SettingWidget);
    } // setupUi

    void retranslateUi(QWidget *SettingWidget)
    {
        SettingWidget->setWindowTitle(QCoreApplication::translate("SettingWidget", "Form", nullptr));
        groupBox->setTitle(QCoreApplication::translate("SettingWidget", "\350\247\206\351\242\221\347\274\226\347\240\201\346\240\274\345\274\217", nullptr));
        radioButton->setText(QCoreApplication::translate("SettingWidget", "H264", nullptr));
        radioButton_2->setText(QCoreApplication::translate("SettingWidget", "H265", nullptr));
        radioButton_3->setText(QCoreApplication::translate("SettingWidget", "VP9", nullptr));
        groupBox_2->setTitle(QCoreApplication::translate("SettingWidget", "\346\226\207\344\273\266\346\240\274\345\274\217", nullptr));
        radioButton_4->setText(QCoreApplication::translate("SettingWidget", "MP4", nullptr));
        radioButton_5->setText(QCoreApplication::translate("SettingWidget", "AVI", nullptr));
        radioButton_6->setText(QCoreApplication::translate("SettingWidget", "MKV", nullptr));
        groupBox_3->setTitle(QCoreApplication::translate("SettingWidget", "\346\216\250\346\265\201\346\240\274\345\274\217", nullptr));
        radioButton_7->setText(QCoreApplication::translate("SettingWidget", "RTSP", nullptr));
        radioButton_8->setText(QCoreApplication::translate("SettingWidget", "RTMP", nullptr));
        radioButton_9->setText(QCoreApplication::translate("SettingWidget", "HTTP", nullptr));
        ensure_btn->setText(QCoreApplication::translate("SettingWidget", "\347\241\256\345\256\232", nullptr));
    } // retranslateUi

};

namespace Ui {
    class SettingWidget: public Ui_SettingWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SETTINGWIDGET_H
