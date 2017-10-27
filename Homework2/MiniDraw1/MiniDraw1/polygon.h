#pragma once
#include <vector>
#include <Figure.h>
using namespace std;
class Polygon :public Figure
{
public:
	Polygon();
	Polygon(const vector<QPoint> points);
	void Draw(QPainter &paint);
	~Polygon();
private:
	vector<QPoint> points_;
};