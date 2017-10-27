#include "imagewidget.h"
#include <iostream>
#include <qpainter.h>
#include <qfiledialog.h>
#include <Eigen/Cholesky>  

ImageWidget::ImageWidget(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);


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
	QImage image_show = QImage((unsigned char *)(image_mat_.data), image_mat_.cols, image_mat_.rows, image_mat_.step, QImage::Format_RGB888);
	QRect rect = QRect(0, 0, image_show.width(), image_show.height());
	painter.drawImage(rect, image_show);

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
	image_mat_ = image_mat_backup_.clone();
	q_points.clear();
	p_points.clear();
	line_array_.clear();
	update();
}



void ImageWidget::SaveAs()
{
	QString filename = QFileDialog::getSaveFileName(this, tr("Save Image"), ".", tr("Images(*.bmp *.png *.jpg)"));
	if (filename.isNull())
	{
		return;
	}

	Mat image_save;
	cvtColor(image_mat_, image_save, CV_RGB2BGR);
	imwrite(filename.toLatin1().data(), image_save);
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

		image_mat_ = cv::imread(fileName.toLatin1().data());
		cvtColor(image_mat_, image_mat_, CV_BGR2RGB);
		image_mat_backup_ = image_mat_.clone();
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

	int width = image_mat_.cols;
	int height = image_mat_.rows;
	ImageWarping imageWarping(p_points, q_points);

	MatIterator_<Vec3b> iter, iterend;
	for (iter = image_mat_.begin<Vec3b>(), iterend = image_mat_.end<Vec3b>(); iter != iterend; ++iter)
	{
		(*iter)[0] = 255;
		(*iter)[1] = 255;
		(*iter)[2] = 255;
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
			image_mat_.at<Vec3b>(x, y) = image_mat_backup_.at<Vec3b>(i, j);
		}
	}


}

void ImageWidget::RBFDraw()
{
	int width = image_mat_.cols;
	int height = image_mat_.rows;
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

	MatIterator_<Vec3b> iter, iterend;
	for (iter = image_mat_.begin<Vec3b>(), iterend = image_mat_.end<Vec3b>(); iter != iterend; ++iter)
	{
		(*iter)[0] = 255;
		(*iter)[1] = 255;
		(*iter)[2] = 255;
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
			image_mat_.at<Vec3b>(x, y) = image_mat_backup_.at<Vec3b>(i, j);
		}
	}
}


