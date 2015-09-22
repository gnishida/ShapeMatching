#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "glew.h"
#include <QtGui/QMainWindow>
#include "ui_MainWindow.h"
#include "GLWidget3D.h"

class MainWindow : public QMainWindow {
	Q_OBJECT

private:
	Ui::MainWindowClass ui;
	GLWidget3D* glWidget;

public:
	MainWindow(QWidget *parent = 0, Qt::WFlags flags = 0);

protected:
	void keyPressEvent(QKeyEvent* e);
	void keyReleaseEvent(QKeyEvent* e);

public slots:
	void onNewSketch();
	void onClear3DModel();
	void onSaveImage();
	void onLoadCamera();
	void onSaveCamera();
	void onResetCamera();
	void onStepFloor();
	void onStepWindow();
	void onViewWireFrame();
	void onAnalyzeRules();
	void onFindMatchingRule();
};

#endif // MAINWINDOW_H
