#pragma once
#include<qpainter.h>

class Line
{
public:
	Line();
	Line(int start_point_x, int start_point_y, int end_point_x, int end_point_y);
	~Line();

	void Draw(QPainter &painter);
private:
	int	start_point_x_, start_point_y_, end_point_x_, end_point_y_;
};