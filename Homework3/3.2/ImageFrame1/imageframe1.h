#ifndef IMAGEFRAME1_H
#define IMAGEFRAME1_H

#include <QtWidgets/QMainWindow>
#include "ui_imageframe1.h"

QT_BEGIN_NAMESPACE
class QAction;
class QMenu;
class ViewWidget;
class QImage;
class QPainter;
class QRect;
class ImageWidget;
QT_END_NAMESPACE



class ImageFrame1 : public QMainWindow
{
	Q_OBJECT

public:
	ImageFrame1(QWidget *parent = 0);
	~ImageFrame1();

private:
	Ui::ImageFrame1Class ui;
	void closeEvent(QCloseEvent *e);
	//void paintEvent(QPaintEvent *paintevent);

private slots:

private:
	void CreateActions();
	void CreateMenus();
	void CreateToolBars();
	void CreateStatusBar();

	QMenu *menu_file_;
	QMenu *menu_edit_;
	QMenu *menu_help;
	QToolBar *toolbar_file_;
	QToolBar *toolbar_edit_;
	QAction *action_new_;
	QAction *action_open_;
	QAction *action_save_;
	QAction *action_saveas_;
	QAction *action_invert_;
	QAction *action_restore_;
	QAction *action_draw_;
	QAction *action_idw_draw_;
	QAction *action_rbf_draw_;

	ImageWidget *imagewidget_;



};

#endif // IMAGEFRAME1_H
