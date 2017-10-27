#pragma once
#include<qpainter.h>
#include "Figure.h"
class Ellipse :public Figure
{
public:
	Ellipse();
	Ellipse(int start_point_x, int start_point_y, int end_point_x, int end_point_y);
	~Ellipse();

	void Draw(QPainter &painter);
private:
	int	start_point_x_, start_point_y_, end_point_x_, end_point_y_;
};