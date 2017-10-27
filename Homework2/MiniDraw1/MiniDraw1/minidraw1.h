#ifndef MINIDRAW1_H
#define MINIDRAW1_H

#include <QtWidgets/QMainWindow>
#include "ui_minidraw1.h"
#include "viewwidget.h"

class MiniDraw1 : public QMainWindow
{
	Q_OBJECT

public:
	MiniDraw1(QWidget *parent = 0);
	~MiniDraw1();

private:
	Ui::MiniDraw1Class ui;

	QAction* hello_world_action_;
	QAction* about_action_;
	QAction* line_action_;
	QAction* rectangle_action_;
	QAction* ellipse_action_;
	QAction* polygon_action_;
	QAction* delete_action_;
	QMenu* main_menu_;
	QToolBar* main_toolbar_;

	void CreateAction();
	void CreateMenu();
	void CreateToolBar();

	ViewWidget*	view_widget_;

};

#endif // MINIDRAW1_H
