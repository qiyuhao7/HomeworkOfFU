#include "polygon.h"

Polygon::Polygon()
{

}

Polygon::Polygon(const vector<QPoint> points)
{
	points_ = points;
}

void Polygon::Draw(QPainter &paint)
{
	paint.drawPolygon(&points_.at(0), points_.size());
}

Polygon::~Polygon()
{

}