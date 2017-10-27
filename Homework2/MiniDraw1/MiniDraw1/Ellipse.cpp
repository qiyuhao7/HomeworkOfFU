#include "Ellipse.h"

Ellipse::Ellipse()
{
}

Ellipse::Ellipse(int start_point_x, int start_point_y, int end_point_x, int end_point_y)
{
	start_point_x_ = start_point_x;
	start_point_y_ = start_point_y;
	end_point_x_ = end_point_x;
	end_point_y_ = end_point_y;
}

void Ellipse::Draw(QPainter &paint)
{
	paint.drawEllipse(start_point_x_, start_point_y_, end_point_x_-start_point_x_, end_point_y_-start_point_y_);
}

Ellipse::~Ellipse()
{

}