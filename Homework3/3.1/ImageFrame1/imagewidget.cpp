#include "imagewidget.h"
#include <iostream>
#include <qpainter.h>
#include <qfiledialog.h>
#include <Eigen/Cholesky>  

ImageWidget::ImageWidget(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	ptr_image_ = new QImage();
	ptr_image_backup_ = new QImage();

}

ImageWidget::~ImageWidget()
{

}

void ImageWidget::paintEvent(QPaintEvent *paintevent)
{
	QPainter painter;
	painter.begin(this);

	painter.setBrush(Qt::lightGray);
	QRect back_rect(0, 0, width(), height());
	painter.drawRect(back_rect);

	// Draw image
	image_start_.rx() = (width() - ptr_image_->width()) / 2;
	image_start_.ry() = (height() - ptr_image_->height()) / 2;
	QRect rect = QRect((width() - ptr_image_->width()) / 2, (height() - ptr_image_->height()) / 2, ptr_image_->width(), ptr_image_->height());

	painter.drawImage(rect, *ptr_image_);
	//painter.setBrush(Qt::black);
	for (size_t i = 0; i < line_array_.size(); i++){
		line_array_[i]->Draw(painter);
	}

	if (draw_status_)
	{
		if (is_draw)
		{
			painter.setPen(QPen(Qt::blue, 2));
			painter.drawLine(start_point_, end_point_);
		}
	}

	painter.end();
	update();
}

void ImageWidget::Restore()
{
	*(ptr_image_) = *(ptr_image_backup_);
	q_points.clear();
	p_points.clear();
	line_array_.clear();
	update();
}

void ImageWidget::Invert()
{
	for (int i = 0; i<ptr_image_->width(); i++)
	{
		for (int j = 0; j<ptr_image_->height(); j++)
		{
			QRgb color = ptr_image_->pixel(i, j);
			ptr_image_->setPixel(i, j, qRgb(255 - qRed(color), 255 - qGreen(color), 255 - qBlue(color)));
		}
	}

	// equivalent member function of class QImage
	// ptr_image_->invertPixels(QImage::InvertRgb);
	update();
}

void ImageWidget::SaveAs()
{
	QString filename = QFileDialog::getSaveFileName(this, tr("Save Image"), ".", tr("Images(*.bmp *.png *.jpg)"));
	if (filename.isNull())
	{
		return;
	}

	ptr_image_->save(filename);
}

void ImageWidget::Save()
{
	SaveAs();
}

void ImageWidget::Open()
{
	// Open file
	QString fileName = QFileDialog::getOpenFileName(this, tr("Read Image"), ".", tr("Images(*.bmp *.png *.jpg)"));

	// Load file
	if (!fileName.isEmpty())
	{
		ptr_image_->load(fileName);
		*(ptr_image_backup_) = *(ptr_image_);
	}

	//std::cout << "image size: " << ptr_image_->width() << ' ' << ptr_image_->height() << std::endl;
	update();
}



void ImageWidget::Draw()
{
	is_draw = true;
}


void ImageWidget::mousePressEvent(QMouseEvent *event)
{
	if (Qt::LeftButton == event->button())
	{
		draw_status_ = true;
		start_point_ = end_point_ = event->pos();
	}
	//std::cout << "mouse pressed" << std::endl;

}

void ImageWidget::mouseMoveEvent(QMouseEvent *event)
{
	
	if (draw_status_)
	{
		end_point_ = event->pos();
	}

}

void ImageWidget::mouseReleaseEvent(QMouseEvent *event)
{
	//std::cout << "mouse released" << std::endl;
	if (is_draw)
	{
		Line* current_line_ = new Line(start_point_.rx(), start_point_.ry(), end_point_.rx(), end_point_.ry());
		line_array_.push_back(current_line_);
		p_points.push_back(QPoint(start_point_.rx()-image_start_.rx(),start_point_.ry()-image_start_.ry()));
		q_points.push_back(QPoint(end_point_.rx() - image_start_.rx(), end_point_.ry() - image_start_.ry()));
		//std::cout << "start: "<<start_point_.rx()<<","<< start_point_.ry()<< std::endl;
		//std::cout << "end: " << end_point_.rx() << "," << end_point_.ry() << std::endl;
		draw_status_ = false;
	}
}

void ImageWidget::IDWDraw()
{
	QImage image_tmp(*(ptr_image_));
	int width = ptr_image_->width();
	int height = ptr_image_->height();
	ImageWarping imageWarping(p_points, q_points);
	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			QRgb color = qRgb(255, 255, 255);
			ptr_image_->setPixel(i, j, color);
		}
	}

	for (int i = 0; i<width; i++)
	{
		for (int j = 0; j<height; j++)
		{
			int x = imageWarping.IDW(QPoint(i, j)).rx();
			int y = imageWarping.IDW(QPoint(i, j)).ry();
			if (x < 0 || y < 0 || x >= width || y >= height)
			{
				//ptr_image_->setPixel(x, y, Qt::white);
				continue;
			}
			QRgb color = image_tmp.pixel(i, j);
			ptr_image_->setPixel(x, y, color);
		}
	}
}

void ImageWidget::RBFDraw()
{
	QImage image_tmp(*(ptr_image_));
	int width = ptr_image_->width();
	int height = ptr_image_->height();
	ImageWarping imageWarping(p_points, q_points);

	imageWarping.getAlpha();

	//Debug Code
	/*
	int n = p_points.size();

	for (int i = 0; i < n; i++)
	{
		std::cout << "p_points:" << p_points[i].rx() << " " << p_points[i].ry() << std::endl;
	}

	for (int i = 0; i < n; i++)
	{
		std::cout << "q_points:" << q_points[i].rx() << " " << q_points[i].ry() << std::endl;
	}

	
	for (int i = 0; i < n; i++)
	{
		std::cout << "q_points:" << q_points[i].rx() << " " << q_points[i].ry() << std::endl;
		QPoint test_point = ImageWidget::RBF(p_points[i]);
		std::cout << "result_points:" << test_point.rx() << " " << test_point.ry() << std::endl;
		double tmp_x = q_points[i].rx() - test_point.rx();
		double tmp_y = q_points[i].ry() - test_point.ry();
		std::cout << tmp_x << " " << tmp_y << std::endl;
	}

	*/

	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			QRgb color = qRgb(255, 255, 255);
			ptr_image_->setPixel(i, j, color);
		}
	}

	for (int i = 0; i<width; i++)
	{
		for (int j = 0; j<height; j++)
		{
			int x = imageWarping.RBF(QPoint(i, j)).rx();
			int y = imageWarping.RBF(QPoint(i, j)).ry();
			if (x < 0 || y < 0 || x >= width || y >= height)
			{
				//ptr_image_->setPixel(x, y, Qt::white);
				continue;
			}
			QRgb color = image_tmp.pixel(i, j);
			ptr_image_->setPixel(x, y, color);
		}
	}
}


