/********************************************************************************
** Form generated from reading UI file 'minidraw1.ui'
**
** Created by: Qt User Interface Compiler version 5.2.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MINIDRAW1_H
#define UI_MINIDRAW1_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MiniDraw1Class
{
public:
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QWidget *centralWidget;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MiniDraw1Class)
    {
        if (MiniDraw1Class->objectName().isEmpty())
            MiniDraw1Class->setObjectName(QStringLiteral("MiniDraw1Class"));
        MiniDraw1Class->resize(600, 400);
        menuBar = new QMenuBar(MiniDraw1Class);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        MiniDraw1Class->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MiniDraw1Class);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MiniDraw1Class->addToolBar(mainToolBar);
        centralWidget = new QWidget(MiniDraw1Class);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        MiniDraw1Class->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(MiniDraw1Class);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MiniDraw1Class->setStatusBar(statusBar);

        retranslateUi(MiniDraw1Class);

        QMetaObject::connectSlotsByName(MiniDraw1Class);
    } // setupUi

    void retranslateUi(QMainWindow *MiniDraw1Class)
    {
        MiniDraw1Class->setWindowTitle(QApplication::translate("MiniDraw1Class", "MiniDraw1", 0));
    } // retranslateUi

};

namespace Ui {
    class MiniDraw1Class: public Ui_MiniDraw1Class {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MINIDRAW1_H
