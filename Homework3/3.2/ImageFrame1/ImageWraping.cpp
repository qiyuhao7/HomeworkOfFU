#include "ImageWarping.h"

ImageWarping::ImageWarping(vector<QPoint> points1, vector<QPoint> points2)
{
	p_points = points1;
	q_points = points2;
}

ImageWarping::~ImageWarping()
{

}

QPoint ImageWarping::IDW(QPoint p)
{
	int n = p_points.size();

	std::vector<double> sigmas;
	double tmp_sigma;
	double tmp_distance = 0;
	for (int i = 0; i < n; i++)
	{
		tmp_distance = (p.rx() - p_points[i].rx())*(p.rx() - p_points[i].rx()) + (p.ry() - p_points[i].ry())*(p.ry() - p_points[i].ry());
		tmp_distance = sqrt(tmp_distance);
		tmp_sigma = pow(1.0 / (tmp_distance), 1.0);
		sigmas.push_back(tmp_sigma);
	}

	double sigma_sum = 0;
	for (int i = 0; i < n; i++)
	{
		sigma_sum += sigmas[i];
	}

	std::vector<double> weights;
	for (int i = 0; i < n; i++)
	{
		weights.push_back(sigmas[i] / sigma_sum);
	}

	std::vector<QPoint> fis;
	for (int i = 0; i < n; i++)
	{
		QPoint tmp_fi = q_points[i];

		tmp_fi.rx() += (p.rx() - p_points[i].rx());
		tmp_fi.ry() += (p.ry() - p_points[i].ry());

		fis.push_back(tmp_fi);
	}

	QPoint result(0, 0);
	double x = 0;
	double y = 0;
	for (int i = 0; i < n; i++)
	{
		x += fis[i].rx()*weights[i];
		y += fis[i].ry()*weights[i];
	}

	result.rx() = x;
	result.ry() = y;

	return result;

}

QPoint ImageWarping::RBF(QPoint p)
{

	int n = p_points.size();
	double fx = 0;
	double fy = 0;
	for (int i = 0; i < n; i++)
	{
		double tmp_fi = distance(p, p_points[i]);
		tmp_fi += ri2s[i];
		tmp_fi = sqrt(tmp_fi);
		fx += ax(i)*tmp_fi;
		fy += ay(i)*tmp_fi;
	}
	fx += p.rx();
	fy += p.ry();
	QPoint result(fx, fy);
	return result;
}

void ImageWarping::getRis()
{
	int n = p_points.size();
	for (int i = 0; i < n; i++)
	{
		double min_distance = 0.0;
		for (int j = 0; j < n; j++)
		{
			if (i == j)
			{
				break;
			}
			else
			{
				double tmp_distance = 0;
				tmp_distance = (p_points[i].rx() - p_points[j].rx())*(p_points[i].rx() - p_points[j].rx()) + (p_points[i].ry() - p_points[j].ry())*(p_points[i].ry() - p_points[j].ry());
				if (min_distance > tmp_distance)
				{
					min_distance = tmp_distance;
				}
			}
		}
		ri2s.push_back(min_distance);
	}
}

void ImageWarping::getAlpha()
{
	int n = p_points.size();
	//std::cout << n << std::endl;
	getRis();

	MatrixXd A(n, n);

	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			double tmp_fi = distance(p_points[i], p_points[j]);
			tmp_fi += ri2s[i];
			tmp_fi = sqrt(tmp_fi);
			A(i, j) = tmp_fi;
		}
	}

	VectorXd bx(n);
	VectorXd by(n);
	for (int i = 0; i < n; i++)
	{
		bx(i) = q_points[i].rx() - p_points[i].rx();
		by(i) = q_points[i].ry() - p_points[i].ry();
	}

	VectorXd tmp_ax(n);
	VectorXd tmp_ay(n);



	tmp_ax = A.fullPivHouseholderQr().solve(bx);
	tmp_ay = A.fullPivHouseholderQr().solve(by);

	//Debug Code
	/*
	std::cout << A << std::endl;

	std::cout << bx << std::endl;
	std::cout << by << std::endl;

	std::cout << tmp_ax << std::endl;
	std::cout << tmp_ay << std::endl;

	std::cout << A*tmp_ax << std::endl;
	std::cout << A*tmp_ay << std::endl;
	*/
	ax = tmp_ax;
	ay = tmp_ay;

}

double ImageWarping::distance(QPoint p1, QPoint p2)
{
	double tmp_distance = 0;
	tmp_distance = (p1.rx() - p2.rx())*(p1.rx() - p2.rx()) + (p1.ry() - p2.ry())*(p1.ry() - p2.ry());
	
	return tmp_distance;
}
