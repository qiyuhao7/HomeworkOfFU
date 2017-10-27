#ifndef IMAGEWARPING_H
#define IMAGEWARPING_H
#include <QWidget>
#include <iostream>
#include <vector>
#include <Eigen/Dense>
#include <Eigen/Cholesky>
using namespace Eigen;
using namespace std;

class ImageWarping
{
public:
	ImageWarping(vector<QPoint> p_points, vector<QPoint> q_points);
	~ImageWarping();

	QPoint IDW(QPoint p);
	QPoint RBF(QPoint p);
	void getAlpha(); //compute RBF's alpha values

private:

	void getRis(); //compute RBF's ri values
	
	double distance(QPoint p1, QPoint p2); //RBF's distance
	vector<QPoint> p_points;
	vector<QPoint> q_points;
	vector<double> ri2s;
	VectorXd ax;
	VectorXd ay;
};


#endif