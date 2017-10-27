#ifndef IDW_H
#define IDW_H

#include <QWidget>

QT_BEGIN_NAMESPACE
class QImage;
class QPainter;
class QPoint;
QT_END_NAMESPACE

class ImW_IDW
{
public:
	ImW_IDW();
	ImW_IDW(vector<QPoint> &m_point_p, vector<QPoint> &m_point_q);

public:
	QPoint f_func(QPoint p);

private:
	double wi_func(QPoint p, int i);
	double delta(QPoint p, QPoint pi);
	double distance(QPoint p, QPoint pi);
	QPoint fi_func(QPoint p, int i);


private:
	double u;
	vector<QPoint> p_points;
	vector<QPoint> q_points;

};

#endif