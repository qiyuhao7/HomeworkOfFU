#ifndef IMAGEWIDGET_H
#define IMAGEWIDGET_H

#include <QWidget>
#include "ui_imagewidget.h"
#include <vector>
#include <qevent.h>
#include <Line.h>
#include <ImageWarping.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>

using namespace cv;

QT_BEGIN_NAMESPACE
class QImage;
class QPainter;
class QPoint;
QT_END_NAMESPACE

class ImageWidget : public QWidget
{
	Q_OBJECT

public:
	ImageWidget(QWidget *parent = 0);
	~ImageWidget();

	void mousePressEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);

protected:
	void paintEvent(QPaintEvent *paintevent);

public slots:
	void Open();
	void Save();
	void SaveAs();

	void Restore();
	void Draw();

	//IDW
	void IDWDraw();

	//RBF
	void RBFDraw();

private:
	Ui::ImageWidget ui;
	std::vector<QPoint> q_points;
	std::vector<QPoint> p_points;

	bool draw_status_;
	QPoint start_point_;
	QPoint end_point_;
	QPoint image_start_;
	//std::vector<QPoint> points;
	std::vector<Line*> line_array_;
	bool is_draw;

	//openCV
	Mat image_mat_;
	Mat image_mat_backup_;

};

#endif // IMAGEWIDGET_H
