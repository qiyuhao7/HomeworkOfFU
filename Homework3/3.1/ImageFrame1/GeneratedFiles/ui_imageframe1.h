/********************************************************************************
** Form generated from reading UI file 'imageframe1.ui'
**
** Created by: Qt User Interface Compiler version 5.2.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_IMAGEFRAME1_H
#define UI_IMAGEFRAME1_H

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

class Ui_ImageFrame1Class
{
public:
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QWidget *centralWidget;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *ImageFrame1Class)
    {
        if (ImageFrame1Class->objectName().isEmpty())
            ImageFrame1Class->setObjectName(QStringLiteral("ImageFrame1Class"));
        ImageFrame1Class->resize(600, 400);
        menuBar = new QMenuBar(ImageFrame1Class);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        ImageFrame1Class->setMenuBar(menuBar);
        mainToolBar = new QToolBar(ImageFrame1Class);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        ImageFrame1Class->addToolBar(mainToolBar);
        centralWidget = new QWidget(ImageFrame1Class);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        ImageFrame1Class->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(ImageFrame1Class);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        ImageFrame1Class->setStatusBar(statusBar);

        retranslateUi(ImageFrame1Class);

        QMetaObject::connectSlotsByName(ImageFrame1Class);
    } // setupUi

    void retranslateUi(QMainWindow *ImageFrame1Class)
    {
        ImageFrame1Class->setWindowTitle(QApplication::translate("ImageFrame1Class", "ImageFrame1", 0));
    } // retranslateUi

};

namespace Ui {
    class ImageFrame1Class: public Ui_ImageFrame1Class {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_IMAGEFRAME1_H
