#pragma once
#include<qpainter.h>
#include "Figure.h"
class Rectangle :public Figure
{
public:
	Rectangle();
	Rectangle(int start_point_x, int start_point_y, int end_point_x, int end_point_y);
	~Rectangle();

	void Draw(QPainter &painter);
private:
	int	start_point_x_, start_point_y_, end_point_x_, end_point_y_;
};