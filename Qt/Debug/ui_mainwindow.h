/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.10.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout_5;
    QVBoxLayout *verticalLayout_4;
    QGridLayout *gridLayout;
    QLabel *ip_label;
    QLineEdit *ip_lineedit;
    QLabel *dataport_label;
    QLineEdit *dataport_lineedit;
    QLabel *videoport_label;
    QLineEdit *videoport_lineedit;
    QPushButton *connect_pushbutton;
    QPushButton *openvid_pushbutton;
    QPushButton *openwrhouse_pushbutton;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *opendata_pushbutton;
    QPushButton *closedata_pushbutton;
    QGridLayout *gridLayout_4;
    QLabel *tempmax_label;
    QLabel *hummax_label;
    QSpinBox *tempmax_spinbox;
    QSpinBox *hummax_spinbox;
    QLabel *tempmin_label;
    QLabel *hummin_label;
    QSpinBox *tempmin_spinbox;
    QSpinBox *hummin_spinbox;
    QVBoxLayout *verticalLayout_3;
    QLabel *message_label;
    QLineEdit *message_lineedit;
    QGroupBox *data_groupbox;
    QVBoxLayout *verticalLayout;
    QGridLayout *gridLayout_2;
    QLabel *light_label;
    QLabel *hum_label;
    QLabel *temp_label;
    QLabel *adc_label;
    QHBoxLayout *horizontalLayout;
    QLabel *x_label;
    QLabel *y_label;
    QLabel *z_label;
    QGroupBox *ctrl_groupbox;
    QGridLayout *gridLayout_3;
    QPushButton *onled_pushbutton;
    QPushButton *offled_pushbutton;
    QPushButton *onfan_pushbutton;
    QPushButton *offfan_pushbutton;
    QPushButton *onsevenled_pushbutton;
    QPushButton *offsevenled_pushbutton;
    QPushButton *onspeaker_pushbutton;
    QPushButton *offspeaker_pushbutton;
    QGroupBox *state_groupbox;
    QVBoxLayout *verticalLayout_2;
    QLabel *led1state_label;
    QLabel *fanstate_label;
    QLabel *sevenledstate_label;
    QLabel *speakerstate_label;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(196, 747);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        verticalLayout_5 = new QVBoxLayout(centralWidget);
        verticalLayout_5->setSpacing(6);
        verticalLayout_5->setContentsMargins(11, 11, 11, 11);
        verticalLayout_5->setObjectName(QStringLiteral("verticalLayout_5"));
        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setSpacing(6);
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        gridLayout = new QGridLayout();
        gridLayout->setSpacing(6);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        ip_label = new QLabel(centralWidget);
        ip_label->setObjectName(QStringLiteral("ip_label"));

        gridLayout->addWidget(ip_label, 0, 0, 1, 1);

        ip_lineedit = new QLineEdit(centralWidget);
        ip_lineedit->setObjectName(QStringLiteral("ip_lineedit"));

        gridLayout->addWidget(ip_lineedit, 0, 1, 1, 1);

        dataport_label = new QLabel(centralWidget);
        dataport_label->setObjectName(QStringLiteral("dataport_label"));
        dataport_label->setEnabled(true);

        gridLayout->addWidget(dataport_label, 1, 0, 1, 1);

        dataport_lineedit = new QLineEdit(centralWidget);
        dataport_lineedit->setObjectName(QStringLiteral("dataport_lineedit"));

        gridLayout->addWidget(dataport_lineedit, 1, 1, 1, 1);

        videoport_label = new QLabel(centralWidget);
        videoport_label->setObjectName(QStringLiteral("videoport_label"));

        gridLayout->addWidget(videoport_label, 2, 0, 1, 1);

        videoport_lineedit = new QLineEdit(centralWidget);
        videoport_lineedit->setObjectName(QStringLiteral("videoport_lineedit"));

        gridLayout->addWidget(videoport_lineedit, 2, 1, 1, 1);

        connect_pushbutton = new QPushButton(centralWidget);
        connect_pushbutton->setObjectName(QStringLiteral("connect_pushbutton"));

        gridLayout->addWidget(connect_pushbutton, 3, 0, 1, 2);

        openvid_pushbutton = new QPushButton(centralWidget);
        openvid_pushbutton->setObjectName(QStringLiteral("openvid_pushbutton"));
        openvid_pushbutton->setEnabled(true);

        gridLayout->addWidget(openvid_pushbutton, 4, 0, 1, 2);

        openwrhouse_pushbutton = new QPushButton(centralWidget);
        openwrhouse_pushbutton->setObjectName(QStringLiteral("openwrhouse_pushbutton"));
        openwrhouse_pushbutton->setEnabled(false);

        gridLayout->addWidget(openwrhouse_pushbutton, 5, 0, 1, 2);


        verticalLayout_4->addLayout(gridLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        opendata_pushbutton = new QPushButton(centralWidget);
        opendata_pushbutton->setObjectName(QStringLiteral("opendata_pushbutton"));
        opendata_pushbutton->setEnabled(false);

        horizontalLayout_2->addWidget(opendata_pushbutton);

        closedata_pushbutton = new QPushButton(centralWidget);
        closedata_pushbutton->setObjectName(QStringLiteral("closedata_pushbutton"));
        closedata_pushbutton->setEnabled(false);

        horizontalLayout_2->addWidget(closedata_pushbutton);


        verticalLayout_4->addLayout(horizontalLayout_2);

        gridLayout_4 = new QGridLayout();
        gridLayout_4->setSpacing(6);
        gridLayout_4->setObjectName(QStringLiteral("gridLayout_4"));
        tempmax_label = new QLabel(centralWidget);
        tempmax_label->setObjectName(QStringLiteral("tempmax_label"));

        gridLayout_4->addWidget(tempmax_label, 0, 0, 1, 1);

        hummax_label = new QLabel(centralWidget);
        hummax_label->setObjectName(QStringLiteral("hummax_label"));

        gridLayout_4->addWidget(hummax_label, 0, 1, 1, 1);

        tempmax_spinbox = new QSpinBox(centralWidget);
        tempmax_spinbox->setObjectName(QStringLiteral("tempmax_spinbox"));
        tempmax_spinbox->setValue(32);

        gridLayout_4->addWidget(tempmax_spinbox, 1, 0, 1, 1);

        hummax_spinbox = new QSpinBox(centralWidget);
        hummax_spinbox->setObjectName(QStringLiteral("hummax_spinbox"));
        hummax_spinbox->setValue(32);

        gridLayout_4->addWidget(hummax_spinbox, 1, 1, 1, 1);

        tempmin_label = new QLabel(centralWidget);
        tempmin_label->setObjectName(QStringLiteral("tempmin_label"));

        gridLayout_4->addWidget(tempmin_label, 2, 0, 1, 1);

        hummin_label = new QLabel(centralWidget);
        hummin_label->setObjectName(QStringLiteral("hummin_label"));

        gridLayout_4->addWidget(hummin_label, 2, 1, 1, 1);

        tempmin_spinbox = new QSpinBox(centralWidget);
        tempmin_spinbox->setObjectName(QStringLiteral("tempmin_spinbox"));
        tempmin_spinbox->setValue(10);

        gridLayout_4->addWidget(tempmin_spinbox, 3, 0, 1, 1);

        hummin_spinbox = new QSpinBox(centralWidget);
        hummin_spinbox->setObjectName(QStringLiteral("hummin_spinbox"));
        hummin_spinbox->setValue(10);

        gridLayout_4->addWidget(hummin_spinbox, 3, 1, 1, 1);


        verticalLayout_4->addLayout(gridLayout_4);

        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        message_label = new QLabel(centralWidget);
        message_label->setObjectName(QStringLiteral("message_label"));

        verticalLayout_3->addWidget(message_label);

        message_lineedit = new QLineEdit(centralWidget);
        message_lineedit->setObjectName(QStringLiteral("message_lineedit"));

        verticalLayout_3->addWidget(message_lineedit);


        verticalLayout_4->addLayout(verticalLayout_3);

        data_groupbox = new QGroupBox(centralWidget);
        data_groupbox->setObjectName(QStringLiteral("data_groupbox"));
        verticalLayout = new QVBoxLayout(data_groupbox);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        gridLayout_2 = new QGridLayout();
        gridLayout_2->setSpacing(6);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        light_label = new QLabel(data_groupbox);
        light_label->setObjectName(QStringLiteral("light_label"));

        gridLayout_2->addWidget(light_label, 1, 1, 1, 1);

        hum_label = new QLabel(data_groupbox);
        hum_label->setObjectName(QStringLiteral("hum_label"));

        gridLayout_2->addWidget(hum_label, 0, 1, 1, 1);

        temp_label = new QLabel(data_groupbox);
        temp_label->setObjectName(QStringLiteral("temp_label"));

        gridLayout_2->addWidget(temp_label, 0, 0, 1, 1);

        adc_label = new QLabel(data_groupbox);
        adc_label->setObjectName(QStringLiteral("adc_label"));

        gridLayout_2->addWidget(adc_label, 1, 0, 1, 1);


        verticalLayout->addLayout(gridLayout_2);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        x_label = new QLabel(data_groupbox);
        x_label->setObjectName(QStringLiteral("x_label"));

        horizontalLayout->addWidget(x_label);

        y_label = new QLabel(data_groupbox);
        y_label->setObjectName(QStringLiteral("y_label"));

        horizontalLayout->addWidget(y_label);

        z_label = new QLabel(data_groupbox);
        z_label->setObjectName(QStringLiteral("z_label"));

        horizontalLayout->addWidget(z_label);


        verticalLayout->addLayout(horizontalLayout);


        verticalLayout_4->addWidget(data_groupbox);

        ctrl_groupbox = new QGroupBox(centralWidget);
        ctrl_groupbox->setObjectName(QStringLiteral("ctrl_groupbox"));
        gridLayout_3 = new QGridLayout(ctrl_groupbox);
        gridLayout_3->setSpacing(6);
        gridLayout_3->setContentsMargins(11, 11, 11, 11);
        gridLayout_3->setObjectName(QStringLiteral("gridLayout_3"));
        onled_pushbutton = new QPushButton(ctrl_groupbox);
        onled_pushbutton->setObjectName(QStringLiteral("onled_pushbutton"));
        onled_pushbutton->setEnabled(false);

        gridLayout_3->addWidget(onled_pushbutton, 0, 0, 1, 1);

        offled_pushbutton = new QPushButton(ctrl_groupbox);
        offled_pushbutton->setObjectName(QStringLiteral("offled_pushbutton"));
        offled_pushbutton->setEnabled(false);

        gridLayout_3->addWidget(offled_pushbutton, 0, 1, 1, 1);

        onfan_pushbutton = new QPushButton(ctrl_groupbox);
        onfan_pushbutton->setObjectName(QStringLiteral("onfan_pushbutton"));
        onfan_pushbutton->setEnabled(false);

        gridLayout_3->addWidget(onfan_pushbutton, 1, 0, 1, 1);

        offfan_pushbutton = new QPushButton(ctrl_groupbox);
        offfan_pushbutton->setObjectName(QStringLiteral("offfan_pushbutton"));
        offfan_pushbutton->setEnabled(false);

        gridLayout_3->addWidget(offfan_pushbutton, 1, 1, 1, 1);

        onsevenled_pushbutton = new QPushButton(ctrl_groupbox);
        onsevenled_pushbutton->setObjectName(QStringLiteral("onsevenled_pushbutton"));
        onsevenled_pushbutton->setEnabled(false);

        gridLayout_3->addWidget(onsevenled_pushbutton, 2, 0, 1, 1);

        offsevenled_pushbutton = new QPushButton(ctrl_groupbox);
        offsevenled_pushbutton->setObjectName(QStringLiteral("offsevenled_pushbutton"));
        offsevenled_pushbutton->setEnabled(false);

        gridLayout_3->addWidget(offsevenled_pushbutton, 2, 1, 1, 1);

        onspeaker_pushbutton = new QPushButton(ctrl_groupbox);
        onspeaker_pushbutton->setObjectName(QStringLiteral("onspeaker_pushbutton"));
        onspeaker_pushbutton->setEnabled(false);

        gridLayout_3->addWidget(onspeaker_pushbutton, 3, 0, 1, 1);

        offspeaker_pushbutton = new QPushButton(ctrl_groupbox);
        offspeaker_pushbutton->setObjectName(QStringLiteral("offspeaker_pushbutton"));
        offspeaker_pushbutton->setEnabled(false);

        gridLayout_3->addWidget(offspeaker_pushbutton, 3, 1, 1, 1);


        verticalLayout_4->addWidget(ctrl_groupbox);

        state_groupbox = new QGroupBox(centralWidget);
        state_groupbox->setObjectName(QStringLiteral("state_groupbox"));
        verticalLayout_2 = new QVBoxLayout(state_groupbox);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        led1state_label = new QLabel(state_groupbox);
        led1state_label->setObjectName(QStringLiteral("led1state_label"));

        verticalLayout_2->addWidget(led1state_label);

        fanstate_label = new QLabel(state_groupbox);
        fanstate_label->setObjectName(QStringLiteral("fanstate_label"));

        verticalLayout_2->addWidget(fanstate_label);

        sevenledstate_label = new QLabel(state_groupbox);
        sevenledstate_label->setObjectName(QStringLiteral("sevenledstate_label"));

        verticalLayout_2->addWidget(sevenledstate_label);

        speakerstate_label = new QLabel(state_groupbox);
        speakerstate_label->setObjectName(QStringLiteral("speakerstate_label"));

        verticalLayout_2->addWidget(speakerstate_label);


        verticalLayout_4->addWidget(state_groupbox);


        verticalLayout_5->addLayout(verticalLayout_4);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 196, 23));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "\346\231\272\350\203\275\344\273\223\345\202\250\347\256\241\347\220\206\347\263\273\347\273\237", nullptr));
        ip_label->setText(QApplication::translate("MainWindow", "\346\234\215\345\212\241\345\231\250\345\234\260\345\235\200\357\274\232", nullptr));
        ip_lineedit->setText(QApplication::translate("MainWindow", "192.168.1.164", nullptr));
        dataport_label->setText(QApplication::translate("MainWindow", "\346\225\260\346\215\256\347\253\257\345\217\243\357\274\232", nullptr));
        dataport_lineedit->setText(QApplication::translate("MainWindow", "6969", nullptr));
        videoport_label->setText(QApplication::translate("MainWindow", "\346\221\204\345\203\217\345\244\264\347\253\257\345\217\243\357\274\232", nullptr));
        videoport_lineedit->setText(QApplication::translate("MainWindow", "6868", nullptr));
        connect_pushbutton->setText(QApplication::translate("MainWindow", "\350\277\236\346\216\245\346\234\215\345\212\241\345\231\250", nullptr));
        openvid_pushbutton->setText(QApplication::translate("MainWindow", "\346\211\223\345\274\200\344\273\223\345\272\223\347\233\221\346\216\247", nullptr));
        openwrhouse_pushbutton->setText(QApplication::translate("MainWindow", "\346\211\223\345\274\200\344\273\223\345\272\223", nullptr));
        opendata_pushbutton->setText(QApplication::translate("MainWindow", "\345\274\200\345\220\257\346\225\260\346\215\256\350\216\267\345\217\226", nullptr));
        closedata_pushbutton->setText(QApplication::translate("MainWindow", "\345\205\263\351\227\255\346\225\260\346\215\256\350\216\267\345\217\226", nullptr));
        tempmax_label->setText(QApplication::translate("MainWindow", "\346\270\251\345\272\246\344\270\212\351\231\220\357\274\232", nullptr));
        hummax_label->setText(QApplication::translate("MainWindow", "\346\271\277\345\272\246\344\270\212\351\231\220\357\274\232", nullptr));
        tempmin_label->setText(QApplication::translate("MainWindow", "\346\270\251\345\272\246\344\270\213\351\231\220\357\274\232", nullptr));
        hummin_label->setText(QApplication::translate("MainWindow", "\346\271\277\345\272\246\344\270\213\351\231\220\357\274\232", nullptr));
        message_label->setText(QApplication::translate("MainWindow", "\347\237\255\344\277\241\345\206\205\345\256\271\357\274\232", nullptr));
        message_lineedit->setText(QApplication::translate("MainWindow", "temp high!!!!", nullptr));
        data_groupbox->setTitle(QApplication::translate("MainWindow", "\350\212\202\347\202\271\346\225\260\346\215\256", nullptr));
        light_label->setText(QApplication::translate("MainWindow", "\345\205\211\347\205\247\357\274\2320", nullptr));
        hum_label->setText(QApplication::translate("MainWindow", "\346\271\277\345\272\246\357\274\2320    ", nullptr));
        temp_label->setText(QApplication::translate("MainWindow", "\346\270\251\345\272\246\357\274\2320", nullptr));
        adc_label->setText(QApplication::translate("MainWindow", "ADC \357\274\2320", nullptr));
        x_label->setText(QApplication::translate("MainWindow", "X\357\274\2320", nullptr));
        y_label->setText(QApplication::translate("MainWindow", "Y\357\274\2320", nullptr));
        z_label->setText(QApplication::translate("MainWindow", "Z\357\274\2320", nullptr));
        ctrl_groupbox->setTitle(QApplication::translate("MainWindow", "\350\256\276\345\244\207\346\216\247\345\210\266", nullptr));
        onled_pushbutton->setText(QApplication::translate("MainWindow", "\346\211\223\345\274\200LED", nullptr));
        offled_pushbutton->setText(QApplication::translate("MainWindow", "\345\205\263\351\227\255LED", nullptr));
        onfan_pushbutton->setText(QApplication::translate("MainWindow", "\346\211\223\345\274\200\351\243\216\346\211\207", nullptr));
        offfan_pushbutton->setText(QApplication::translate("MainWindow", "\345\205\263\351\227\255\351\243\216\346\211\207", nullptr));
        onsevenled_pushbutton->setText(QApplication::translate("MainWindow", "\346\211\223\345\274\200\346\225\260\347\240\201\347\256\241", nullptr));
        offsevenled_pushbutton->setText(QApplication::translate("MainWindow", "\345\205\263\351\227\255\346\225\260\347\240\201\347\256\241", nullptr));
        onspeaker_pushbutton->setText(QApplication::translate("MainWindow", "\346\211\223\345\274\200\350\234\202\351\270\243\345\231\250", nullptr));
        offspeaker_pushbutton->setText(QApplication::translate("MainWindow", "\345\205\263\351\227\255\350\234\202\351\270\243\345\231\250", nullptr));
        state_groupbox->setTitle(QApplication::translate("MainWindow", "\350\256\276\345\244\207\347\212\266\346\200\201", nullptr));
        led1state_label->setText(QApplication::translate("MainWindow", "LED\347\212\266\346\200\201   \357\274\232\346\234\252\347\237\245", nullptr));
        fanstate_label->setText(QApplication::translate("MainWindow", "\351\243\216\346\211\207\347\212\266\346\200\201  \357\274\232\346\234\252\347\237\245", nullptr));
        sevenledstate_label->setText(QApplication::translate("MainWindow", "\346\225\260\347\240\201\347\256\241\347\212\266\346\200\201\357\274\232\346\234\252\347\237\245", nullptr));
        speakerstate_label->setText(QApplication::translate("MainWindow", "\346\212\245\350\255\246\345\231\250\347\212\266\346\200\201\357\274\232\346\234\252\347\237\245", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
