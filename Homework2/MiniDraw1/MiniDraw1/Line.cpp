#include "Line.h"

Line::Line()
{
}

Line::Line(int start_point_x, int start_point_y, int end_point_x, int end_point_y)
{
	start_point_x_ = start_point_x;
	start_point_y_ = start_point_y;
	end_point_x_ = end_point_x;
	end_point_y_ = end_point_y;
}

void Line::Draw(QPainter &paint)
{
	paint.drawLine(start_point_x_, start_point_y_, end_point_x_, end_point_y_);
}

Line::~Line()
{

}

