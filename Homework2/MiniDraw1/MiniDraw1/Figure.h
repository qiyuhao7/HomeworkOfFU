#pragma once
#include<qpainter.h>
class Figure
{
public:
	Figure();
	virtual void Draw(QPainter &paint)=0;
	virtual ~Figure();

	enum FigureType
	{
		kDefault = 0,
		kLine = 1,
		kRectangle = 2,
		kEllipse = 3,
		kPolygon = 4,
	};

};