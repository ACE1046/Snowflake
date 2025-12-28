/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QGraphicsView *graphicsView;
    QComboBox *comboBox;
    QComboBox *comboBox_2;
    QLabel *label_1;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *label_4;
    QLabel *label_5;
    QLabel *label_6;
    QCheckBox *checkBox_1;
    QCheckBox *checkBox_2;
    QCheckBox *checkBox_3;
    QCheckBox *checkBox_4;
    QCheckBox *checkBox_5;
    QCheckBox *checkBox_6;
    QCheckBox *checkBox_7;
    QCheckBox *cbShuffle;
    QPushButton *pushButton;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(534, 283);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        graphicsView = new QGraphicsView(centralWidget);
        graphicsView->setObjectName(QStringLiteral("graphicsView"));
        graphicsView->setGeometry(QRect(10, 10, 261, 261));
        comboBox = new QComboBox(centralWidget);
        comboBox->setObjectName(QStringLiteral("comboBox"));
        comboBox->setGeometry(QRect(290, 30, 231, 22));
        comboBox->setMaxVisibleItems(20);
        comboBox_2 = new QComboBox(centralWidget);
        comboBox_2->setObjectName(QStringLiteral("comboBox_2"));
        comboBox_2->setGeometry(QRect(290, 60, 231, 22));
        comboBox_2->setMaxVisibleItems(20);
        label_1 = new QLabel(centralWidget);
        label_1->setObjectName(QStringLiteral("label_1"));
        label_1->setGeometry(QRect(290, 90, 31, 31));
        label_1->setAutoFillBackground(false);
        label_1->setStyleSheet(QStringLiteral("background-color: rgb(0, 0, 0);"));
        label_2 = new QLabel(centralWidget);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(330, 90, 31, 31));
        label_2->setStyleSheet(QStringLiteral("background-color: rgb(0, 0, 0);"));
        label_3 = new QLabel(centralWidget);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(370, 90, 31, 31));
        label_3->setStyleSheet(QStringLiteral("background-color: rgb(0, 0, 0);"));
        label_4 = new QLabel(centralWidget);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(410, 90, 31, 31));
        label_4->setStyleSheet(QStringLiteral("background-color: rgb(0, 0, 0);"));
        label_5 = new QLabel(centralWidget);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setGeometry(QRect(450, 90, 31, 31));
        label_5->setStyleSheet(QStringLiteral("background-color: rgb(0, 0, 0);"));
        label_6 = new QLabel(centralWidget);
        label_6->setObjectName(QStringLiteral("label_6"));
        label_6->setGeometry(QRect(490, 90, 31, 31));
        label_6->setStyleSheet(QStringLiteral("background-color: rgb(0, 0, 0);"));
        checkBox_1 = new QCheckBox(centralWidget);
        checkBox_1->setObjectName(QStringLiteral("checkBox_1"));
        checkBox_1->setGeometry(QRect(290, 130, 70, 17));
        checkBox_2 = new QCheckBox(centralWidget);
        checkBox_2->setObjectName(QStringLiteral("checkBox_2"));
        checkBox_2->setGeometry(QRect(290, 150, 70, 17));
        checkBox_3 = new QCheckBox(centralWidget);
        checkBox_3->setObjectName(QStringLiteral("checkBox_3"));
        checkBox_3->setGeometry(QRect(290, 170, 70, 17));
        checkBox_4 = new QCheckBox(centralWidget);
        checkBox_4->setObjectName(QStringLiteral("checkBox_4"));
        checkBox_4->setGeometry(QRect(290, 190, 70, 17));
        checkBox_5 = new QCheckBox(centralWidget);
        checkBox_5->setObjectName(QStringLiteral("checkBox_5"));
        checkBox_5->setGeometry(QRect(290, 210, 70, 17));
        checkBox_6 = new QCheckBox(centralWidget);
        checkBox_6->setObjectName(QStringLiteral("checkBox_6"));
        checkBox_6->setGeometry(QRect(290, 230, 70, 17));
        checkBox_7 = new QCheckBox(centralWidget);
        checkBox_7->setObjectName(QStringLiteral("checkBox_7"));
        checkBox_7->setGeometry(QRect(290, 250, 70, 17));
        cbShuffle = new QCheckBox(centralWidget);
        cbShuffle->setObjectName(QStringLiteral("cbShuffle"));
        cbShuffle->setGeometry(QRect(290, 10, 211, 17));
        pushButton = new QPushButton(centralWidget);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(440, 240, 75, 23));
        MainWindow->setCentralWidget(centralWidget);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "Star decoration test", 0));
        comboBox->clear();
        comboBox->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "Linear wave (\320\222\320\276\320\273\320\275\320\260 \320\277\320\276 \320\264\320\270\320\276\320\264\320\260\320\274)", 0)
         << QApplication::translate("MainWindow", "Wave radial (\320\240\320\260\320\264\320\270\320\260\320\273\321\214\320\275\320\260\321\217 \320\262\320\276\320\273\320\275\320\260)", 0)
         << QApplication::translate("MainWindow", "Wave rotate (\320\222\321\200\320\260\321\211\320\260\321\216\321\211\320\260\321\217\321\201\321\217 \320\262\320\276\320\273\320\275\320\260)", 0)
         << QApplication::translate("MainWindow", "Flux capacitor (\320\237\320\276\321\202\320\276\320\272\320\276\320\262\321\213\320\271 \320\275\320\260\320\272\320\276\320\277\320\270\321\202\320\265\320\273\321\214)", 0)
         << QApplication::translate("MainWindow", "Flash (\320\222\321\201\320\277\321\213\321\210\320\272\320\270)", 0)
         << QApplication::translate("MainWindow", "Running lights (\320\221\320\265\320\263\320\260\321\216\321\211\320\270\320\265 \320\276\320\263\320\275\320\270)", 0)
         << QApplication::translate("MainWindow", "Spiral (\320\241\320\277\320\270\321\200\320\260\320\273\321\214)", 0)
         << QApplication::translate("MainWindow", "Tree (\320\224\320\265\321\200\320\265\320\262\321\214\321\217)", 0)
         << QApplication::translate("MainWindow", "Flag (\320\244\320\273\320\260\320\263)", 0)
         << QApplication::translate("MainWindow", "Flat wave (\320\237\320\273\320\276\321\201\320\272\320\260\321\217 \320\262\320\276\320\273\320\275\320\260)", 0)
         << QApplication::translate("MainWindow", "Monochrome (\320\236\320\264\320\270\320\275 \321\206\320\262\320\265\321\202)", 0)
         << QApplication::translate("MainWindow", "Running lights 2 (\320\221\320\265\320\263\320\260\321\216\321\211\320\270\320\265 \320\276\320\263\320\275\320\270 2)", 0)
         << QApplication::translate("MainWindow", "Spotlight (\320\237\321\200\320\276\320\266\320\265\320\272\321\202\320\276\321\200\321\213)", 0)
         << QApplication::translate("MainWindow", "Arrows (\320\241\321\202\321\200\320\265\320\273\320\272\320\270)", 0)
         << QApplication::translate("MainWindow", "Spotlight mix (\320\237\321\200\320\276\320\266\320\265\320\272\321\202\320\276\321\200 2)", 0)
         << QApplication::translate("MainWindow", "Plasma (\320\237\320\273\320\260\320\267\320\274\320\260)", 0)
         << QApplication::translate("MainWindow", "Multi rotate (\320\234\321\203\320\273\321\214\321\202\320\270\320\262\321\200\320\260\321\211\320\265\320\275\320\270\320\265)", 0)
         << QApplication::translate("MainWindow", "Bounce (\320\243\320\277\321\200\321\203\320\263\320\276\321\201\321\202\321\214)", 0)
         << QApplication::translate("MainWindow", "Fill (\320\227\320\260\320\277\320\276\320\273\320\275\320\265\320\275\320\270\320\265)", 0)
         << QApplication::translate("MainWindow", "Fireworks (\320\244\320\265\320\271\320\265\321\200\320\262\320\265\321\200\320\272)", 0)
         << QApplication::translate("MainWindow", "Propeller (\320\237\321\200\320\276\320\277\320\265\320\273\320\273\320\265\321\200)", 0)
         << QApplication::translate("MainWindow", "Diamond (\320\220\320\273\320\274\320\260\320\267)", 0)
        );
        comboBox_2->clear();
        comboBox_2->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "Rainbow", 0)
         << QApplication::translate("MainWindow", "Light blue", 0)
         << QApplication::translate("MainWindow", "Dark blue", 0)
         << QApplication::translate("MainWindow", "Fire", 0)
         << QApplication::translate("MainWindow", "Green flash", 0)
         << QApplication::translate("MainWindow", "Red/Yel/Gr flash", 0)
         << QApplication::translate("MainWindow", "Pastel", 0)
         << QApplication::translate("MainWindow", "Blue/Yellow", 0)
         << QApplication::translate("MainWindow", "Coca-Cola", 0)
         << QApplication::translate("MainWindow", "Russia", 0)
         << QApplication::translate("MainWindow", "Chrome", 0)
         << QApplication::translate("MainWindow", "Green fade", 0)
         << QApplication::translate("MainWindow", "Red/Yel/Gr fade", 0)
         << QApplication::translate("MainWindow", "Police", 0)
         << QApplication::translate("MainWindow", "Random", 0)
        );
        label_1->setText(QString());
        label_2->setText(QString());
        label_3->setText(QString());
        label_4->setText(QString());
        label_5->setText(QString());
        label_6->setText(QString());
        checkBox_1->setText(QApplication::translate("MainWindow", "Param 1", 0));
        checkBox_2->setText(QApplication::translate("MainWindow", "Param 2", 0));
        checkBox_3->setText(QApplication::translate("MainWindow", "Param 3", 0));
        checkBox_4->setText(QApplication::translate("MainWindow", "Param 4", 0));
        checkBox_5->setText(QApplication::translate("MainWindow", "Param 5", 0));
        checkBox_6->setText(QApplication::translate("MainWindow", "Param 6", 0));
        checkBox_7->setText(QApplication::translate("MainWindow", "Param 7", 0));
        cbShuffle->setText(QApplication::translate("MainWindow", "Shuffle (\321\201\320\273\321\203\321\207\320\260\320\271\320\275\320\276\320\265)", 0));
        pushButton->setText(QApplication::translate("MainWindow", "Save", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
