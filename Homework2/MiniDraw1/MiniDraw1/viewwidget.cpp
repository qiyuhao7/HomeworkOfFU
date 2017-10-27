#include "viewwidget.h"


ViewWidget::ViewWidget(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	draw_status_ = false;
	figure_type_ = Figure::kDefault;
}



void ViewWidget::mousePressEvent(QMouseEvent *event)
{
	if (Qt::LeftButton == event->button())
	{
		draw_status_ = true;
		start_point_ = end_point_ = event->pos();
	}

	

	if (figure_type_ == Figure::kPolygon){
		points.push_back(start_point_);
	}
}

void ViewWidget::mouseMoveEvent(QMouseEvent *event)
{

	if (draw_status_)
	{
		end_point_ = event->pos();
	}

}

void ViewWidget::mouseReleaseEvent(QMouseEvent *event)
{
	Figure* current_figure_ = NULL;

	switch (figure_type_)
	{
	case Figure::kDefault:
		break;
	case Figure::kLine:
		current_figure_ = new Line(start_point_.rx(), start_point_.ry(), end_point_.rx(), end_point_.ry());
		figure_array_.push_back(current_figure_);
		draw_status_ = false;
		break;
	case Figure::kEllipse:
		current_figure_ = new Ellipse(start_point_.rx(), start_point_.ry(), end_point_.rx(), end_point_.ry());
		figure_array_.push_back(current_figure_);
		draw_status_ = false;
		break;
	case Figure::kRectangle:
		current_figure_ = new Rectangle(start_point_.rx(), start_point_.ry(), end_point_.rx(), end_point_.ry());
		figure_array_.push_back(current_figure_);
		draw_status_ = false;
		break;
	case Figure::kPolygon:
		points.push_back(end_point_);
		if (Qt::RightButton == event->button())
		{
			current_figure_ = new Polygon(points);
			figure_array_.push_back(current_figure_);
			points.clear();
		}
		break;
	default:
		break;
	}

	current_figure_ = NULL;
	
}

void ViewWidget::paintEvent(QPaintEvent *)
{
	QPainter painter(this);


	painter.begin(this);

	for (size_t i = 0; i < figure_array_.size(); i++){
		figure_array_[i]->Draw(painter);
	}


	if (draw_status_){
		switch (figure_type_)
		{
		case Figure::kDefault:
			break;
		case Figure::kLine:
			painter.drawLine(start_point_, end_point_);
			break;
		case Figure::kEllipse:
			painter.drawEllipse(start_point_.rx(), start_point_.ry(), end_point_.rx() - start_point_.rx(), end_point_.ry() - start_point_.ry());
			break;
		case Figure::kRectangle:
			painter.drawRect(start_point_.rx(), start_point_.ry(), end_point_.rx() - start_point_.rx(), end_point_.ry() - start_point_.ry());
			break;
		case Figure::kPolygon:
			if (points.size() == 0)
			{
				break;
			}
			QPoint pre = points[0];
			for (int i = 1; i < points.size(); i++)
			{
				painter.drawLine(pre, points[i]);
				pre = points[i];
			}
			break;
		}
	}
	
	painter.end();
	
	update();
}

ViewWidget::~ViewWidget()
{
	for (size_t i = 0; i < figure_array_.size(); i++)
	{
		if (figure_array_[i])
		{
			delete figure_array_[i];
			figure_array_[i] = NULL;
		}
	}
}

void ViewWidget::set_figure_type_to_line()
{
	figure_type_ = Figure::kLine;
}

void ViewWidget::set_figure_type_to_ellipse()
{
	figure_type_ = Figure::kEllipse;
}

void ViewWidget::set_figure_type_to_rectangle()
{
	figure_type_ = Figure::kRectangle;
}

void ViewWidget::set_figure_type_to_polygon()
{
	figure_type_ = Figure::kPolygon;
}

void ViewWidget::delete_figure()
{
	if (!figure_array_.empty())
	{
		delete figure_array_.back();
		figure_array_.pop_back();
	}
}

