#ifndef VIEWWIDGET_H
#define VIEWWIDGET_H

#include <QWidget>
#include<qevent.h>
#include<qpainter.h>
#include "ui_viewwidget.h"
#include <vector>
#include "Line.h"
#include "Figure.h"
#include "Ellipse.h"
#include "Rectangle.h"
#include "Polygon.h"

using namespace std;



class ViewWidget : public QWidget
{
	Q_OBJECT

public:
	ViewWidget(QWidget *parent = 0);
	~ViewWidget();

	void mousePressEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
	void paintEvent(QPaintEvent *);
	void set_figure_type_to_line();
	void set_figure_type_to_ellipse();
	void set_figure_type_to_rectangle();
	void set_figure_type_to_polygon();
	void delete_figure();

private:
	Ui::ViewWidget ui;

	bool draw_status_;
	QPoint start_point_;
	QPoint end_point_;
	vector<QPoint> points;
	Figure::FigureType figure_type_;
	vector<Figure*> figure_array_; 

};


#endif // VIEWWIDGET_H
