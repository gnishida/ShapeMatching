/********************************************************************************
** Form generated from reading UI file 'MainWindow.ui'
**
** Created: Tue Sep 22 09:43:12 2015
**      by: Qt User Interface Compiler version 4.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QStatusBar>
#include <QtGui/QToolBar>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindowClass
{
public:
    QAction *actionExit;
    QAction *actionGreedyInverse;
    QAction *actionSaveImage;
    QAction *actionModeSketch;
    QAction *actionMode3DView;
    QAction *actionRandomGeneration_2;
    QAction *actionRandomGeneration;
    QAction *actionLoadSketch;
    QAction *actionSaveSketch;
    QAction *actionNewSketch;
    QAction *actionPenColorBranch;
    QAction *actionPenColorLeaf;
    QAction *actionPenWidth20;
    QAction *actionPenWidth10;
    QAction *actionPenWidth5;
    QAction *actionLoadCamera;
    QAction *actionSaveCamera;
    QAction *actionResetCamera;
    QAction *actionOptions;
    QAction *actionStepFloor;
    QAction *actionStepWindow;
    QAction *actionViewWireFrame;
    QAction *actionAnalyzeRules;
    QAction *actionFindMatchingRule;
    QAction *actionClear3DModel;
    QWidget *centralWidget;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menuStep;
    QMenu *menuView;
    QMenu *menuTest;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindowClass)
    {
        if (MainWindowClass->objectName().isEmpty())
            MainWindowClass->setObjectName(QString::fromUtf8("MainWindowClass"));
        MainWindowClass->resize(800, 600);
        actionExit = new QAction(MainWindowClass);
        actionExit->setObjectName(QString::fromUtf8("actionExit"));
        actionGreedyInverse = new QAction(MainWindowClass);
        actionGreedyInverse->setObjectName(QString::fromUtf8("actionGreedyInverse"));
        actionSaveImage = new QAction(MainWindowClass);
        actionSaveImage->setObjectName(QString::fromUtf8("actionSaveImage"));
        actionModeSketch = new QAction(MainWindowClass);
        actionModeSketch->setObjectName(QString::fromUtf8("actionModeSketch"));
        actionModeSketch->setCheckable(true);
        actionModeSketch->setChecked(true);
        actionMode3DView = new QAction(MainWindowClass);
        actionMode3DView->setObjectName(QString::fromUtf8("actionMode3DView"));
        actionMode3DView->setCheckable(true);
        actionRandomGeneration_2 = new QAction(MainWindowClass);
        actionRandomGeneration_2->setObjectName(QString::fromUtf8("actionRandomGeneration_2"));
        actionRandomGeneration = new QAction(MainWindowClass);
        actionRandomGeneration->setObjectName(QString::fromUtf8("actionRandomGeneration"));
        actionLoadSketch = new QAction(MainWindowClass);
        actionLoadSketch->setObjectName(QString::fromUtf8("actionLoadSketch"));
        actionSaveSketch = new QAction(MainWindowClass);
        actionSaveSketch->setObjectName(QString::fromUtf8("actionSaveSketch"));
        actionNewSketch = new QAction(MainWindowClass);
        actionNewSketch->setObjectName(QString::fromUtf8("actionNewSketch"));
        actionPenColorBranch = new QAction(MainWindowClass);
        actionPenColorBranch->setObjectName(QString::fromUtf8("actionPenColorBranch"));
        actionPenColorBranch->setCheckable(true);
        actionPenColorLeaf = new QAction(MainWindowClass);
        actionPenColorLeaf->setObjectName(QString::fromUtf8("actionPenColorLeaf"));
        actionPenColorLeaf->setCheckable(true);
        actionPenWidth20 = new QAction(MainWindowClass);
        actionPenWidth20->setObjectName(QString::fromUtf8("actionPenWidth20"));
        actionPenWidth20->setCheckable(true);
        actionPenWidth10 = new QAction(MainWindowClass);
        actionPenWidth10->setObjectName(QString::fromUtf8("actionPenWidth10"));
        actionPenWidth10->setCheckable(true);
        actionPenWidth5 = new QAction(MainWindowClass);
        actionPenWidth5->setObjectName(QString::fromUtf8("actionPenWidth5"));
        actionPenWidth5->setCheckable(true);
        actionLoadCamera = new QAction(MainWindowClass);
        actionLoadCamera->setObjectName(QString::fromUtf8("actionLoadCamera"));
        actionSaveCamera = new QAction(MainWindowClass);
        actionSaveCamera->setObjectName(QString::fromUtf8("actionSaveCamera"));
        actionResetCamera = new QAction(MainWindowClass);
        actionResetCamera->setObjectName(QString::fromUtf8("actionResetCamera"));
        actionOptions = new QAction(MainWindowClass);
        actionOptions->setObjectName(QString::fromUtf8("actionOptions"));
        actionStepFloor = new QAction(MainWindowClass);
        actionStepFloor->setObjectName(QString::fromUtf8("actionStepFloor"));
        actionStepFloor->setCheckable(true);
        actionStepWindow = new QAction(MainWindowClass);
        actionStepWindow->setObjectName(QString::fromUtf8("actionStepWindow"));
        actionStepWindow->setCheckable(true);
        actionViewWireFrame = new QAction(MainWindowClass);
        actionViewWireFrame->setObjectName(QString::fromUtf8("actionViewWireFrame"));
        actionViewWireFrame->setCheckable(true);
        actionAnalyzeRules = new QAction(MainWindowClass);
        actionAnalyzeRules->setObjectName(QString::fromUtf8("actionAnalyzeRules"));
        actionFindMatchingRule = new QAction(MainWindowClass);
        actionFindMatchingRule->setObjectName(QString::fromUtf8("actionFindMatchingRule"));
        actionClear3DModel = new QAction(MainWindowClass);
        actionClear3DModel->setObjectName(QString::fromUtf8("actionClear3DModel"));
        centralWidget = new QWidget(MainWindowClass);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        MainWindowClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindowClass);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 800, 21));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QString::fromUtf8("menuFile"));
        menuStep = new QMenu(menuBar);
        menuStep->setObjectName(QString::fromUtf8("menuStep"));
        menuView = new QMenu(menuBar);
        menuView->setObjectName(QString::fromUtf8("menuView"));
        menuTest = new QMenu(menuBar);
        menuTest->setObjectName(QString::fromUtf8("menuTest"));
        MainWindowClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindowClass);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        MainWindowClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindowClass);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MainWindowClass->setStatusBar(statusBar);

        menuBar->addAction(menuFile->menuAction());
        menuBar->addAction(menuStep->menuAction());
        menuBar->addAction(menuView->menuAction());
        menuBar->addAction(menuTest->menuAction());
        menuFile->addAction(actionNewSketch);
        menuFile->addAction(actionClear3DModel);
        menuFile->addSeparator();
        menuFile->addAction(actionSaveImage);
        menuFile->addSeparator();
        menuFile->addAction(actionLoadCamera);
        menuFile->addAction(actionSaveCamera);
        menuFile->addAction(actionResetCamera);
        menuFile->addSeparator();
        menuFile->addAction(actionExit);
        menuStep->addAction(actionStepFloor);
        menuStep->addAction(actionStepWindow);
        menuView->addAction(actionViewWireFrame);
        menuTest->addAction(actionAnalyzeRules);
        menuTest->addAction(actionFindMatchingRule);

        retranslateUi(MainWindowClass);

        QMetaObject::connectSlotsByName(MainWindowClass);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindowClass)
    {
        MainWindowClass->setWindowTitle(QApplication::translate("MainWindowClass", "Sketch Application", 0, QApplication::UnicodeUTF8));
        actionExit->setText(QApplication::translate("MainWindowClass", "&Exit", 0, QApplication::UnicodeUTF8));
        actionExit->setShortcut(QApplication::translate("MainWindowClass", "Ctrl+Q", 0, QApplication::UnicodeUTF8));
        actionGreedyInverse->setText(QApplication::translate("MainWindowClass", "&Greedy Inverse", 0, QApplication::UnicodeUTF8));
        actionGreedyInverse->setShortcut(QApplication::translate("MainWindowClass", "Ctrl+I", 0, QApplication::UnicodeUTF8));
        actionSaveImage->setText(QApplication::translate("MainWindowClass", "Save Image", 0, QApplication::UnicodeUTF8));
        actionSaveImage->setShortcut(QApplication::translate("MainWindowClass", "Ctrl+S", 0, QApplication::UnicodeUTF8));
        actionModeSketch->setText(QApplication::translate("MainWindowClass", "Sketch", 0, QApplication::UnicodeUTF8));
        actionMode3DView->setText(QApplication::translate("MainWindowClass", "3D View", 0, QApplication::UnicodeUTF8));
        actionRandomGeneration_2->setText(QApplication::translate("MainWindowClass", "Random Generation", 0, QApplication::UnicodeUTF8));
        actionRandomGeneration->setText(QApplication::translate("MainWindowClass", "&Random Generation", 0, QApplication::UnicodeUTF8));
        actionRandomGeneration->setShortcut(QApplication::translate("MainWindowClass", "Ctrl+R", 0, QApplication::UnicodeUTF8));
        actionLoadSketch->setText(QApplication::translate("MainWindowClass", "&Load Sketch", 0, QApplication::UnicodeUTF8));
        actionLoadSketch->setShortcut(QApplication::translate("MainWindowClass", "Ctrl+O", 0, QApplication::UnicodeUTF8));
        actionSaveSketch->setText(QApplication::translate("MainWindowClass", "&Save Sketch", 0, QApplication::UnicodeUTF8));
        actionSaveSketch->setShortcut(QApplication::translate("MainWindowClass", "Ctrl+S", 0, QApplication::UnicodeUTF8));
        actionNewSketch->setText(QApplication::translate("MainWindowClass", "&New Sketch", 0, QApplication::UnicodeUTF8));
        actionNewSketch->setShortcut(QApplication::translate("MainWindowClass", "Ctrl+N", 0, QApplication::UnicodeUTF8));
        actionPenColorBranch->setText(QApplication::translate("MainWindowClass", "Branch", 0, QApplication::UnicodeUTF8));
        actionPenColorLeaf->setText(QApplication::translate("MainWindowClass", "Leaf", 0, QApplication::UnicodeUTF8));
        actionPenWidth20->setText(QApplication::translate("MainWindowClass", "20", 0, QApplication::UnicodeUTF8));
        actionPenWidth10->setText(QApplication::translate("MainWindowClass", "10", 0, QApplication::UnicodeUTF8));
        actionPenWidth5->setText(QApplication::translate("MainWindowClass", "5", 0, QApplication::UnicodeUTF8));
        actionLoadCamera->setText(QApplication::translate("MainWindowClass", "Load Camera", 0, QApplication::UnicodeUTF8));
        actionSaveCamera->setText(QApplication::translate("MainWindowClass", "Save Camera", 0, QApplication::UnicodeUTF8));
        actionResetCamera->setText(QApplication::translate("MainWindowClass", "Reset Camera", 0, QApplication::UnicodeUTF8));
        actionOptions->setText(QApplication::translate("MainWindowClass", "Options", 0, QApplication::UnicodeUTF8));
        actionStepFloor->setText(QApplication::translate("MainWindowClass", "Floor", 0, QApplication::UnicodeUTF8));
        actionStepWindow->setText(QApplication::translate("MainWindowClass", "Window", 0, QApplication::UnicodeUTF8));
        actionViewWireFrame->setText(QApplication::translate("MainWindowClass", "WireFrame", 0, QApplication::UnicodeUTF8));
        actionViewWireFrame->setShortcut(QApplication::translate("MainWindowClass", "W", 0, QApplication::UnicodeUTF8));
        actionAnalyzeRules->setText(QApplication::translate("MainWindowClass", "Analyze Rules", 0, QApplication::UnicodeUTF8));
        actionAnalyzeRules->setShortcut(QApplication::translate("MainWindowClass", "Ctrl+R", 0, QApplication::UnicodeUTF8));
        actionFindMatchingRule->setText(QApplication::translate("MainWindowClass", "Find Matching Rule", 0, QApplication::UnicodeUTF8));
        actionFindMatchingRule->setShortcut(QApplication::translate("MainWindowClass", "Ctrl+M", 0, QApplication::UnicodeUTF8));
        actionClear3DModel->setText(QApplication::translate("MainWindowClass", "Clear 3D Model", 0, QApplication::UnicodeUTF8));
        menuFile->setTitle(QApplication::translate("MainWindowClass", "&File", 0, QApplication::UnicodeUTF8));
        menuStep->setTitle(QApplication::translate("MainWindowClass", "Step", 0, QApplication::UnicodeUTF8));
        menuView->setTitle(QApplication::translate("MainWindowClass", "View", 0, QApplication::UnicodeUTF8));
        menuTest->setTitle(QApplication::translate("MainWindowClass", "Tool", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindowClass: public Ui_MainWindowClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
