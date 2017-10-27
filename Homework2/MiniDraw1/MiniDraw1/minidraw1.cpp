#include "minidraw1.h"

MiniDraw1::MiniDraw1(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	view_widget_ = new ViewWidget();
	setCentralWidget(view_widget_);
	CreateAction();
	CreateMenu();
	CreateToolBar();
}

MiniDraw1::~MiniDraw1()
{

}

void MiniDraw1::CreateAction()
{
	line_action_ = new QAction(tr("&Line"), this);
	connect(line_action_, &QAction::triggered, view_widget_, &ViewWidget::set_figure_type_to_line);

	ellipse_action_ = new QAction(tr("&Ellipse"), this);
	connect(ellipse_action_, &QAction::triggered, view_widget_, &ViewWidget::set_figure_type_to_ellipse);

	rectangle_action_ = new QAction(tr("&Rectangle"), this);
	connect(rectangle_action_, &QAction::triggered, view_widget_, &ViewWidget::set_figure_type_to_rectangle);

	polygon_action_ = new QAction(tr("&Polygon"), this);
	connect(polygon_action_, &QAction::triggered, view_widget_, &ViewWidget::set_figure_type_to_polygon);

	delete_action_ = new QAction(tr("&Delete"), this);
	connect(delete_action_, &QAction::triggered, view_widget_, &ViewWidget::delete_figure);

}

void MiniDraw1::CreateMenu()
{
	main_menu_ = menuBar()->addMenu(tr("&Figure Tool"));
	main_menu_->addAction(line_action_);
	main_menu_->addAction(ellipse_action_);
	main_menu_->addAction(rectangle_action_);
	main_menu_->addAction(polygon_action_);
}

void MiniDraw1::CreateToolBar()
{
	
	main_toolbar_ = addToolBar(tr("&Main"));
	main_toolbar_->addAction(line_action_);
	main_toolbar_->addAction(rectangle_action_);
	main_toolbar_->addAction(ellipse_action_);
	main_toolbar_->addAction(polygon_action_);
	main_toolbar_->addAction(delete_action_);

}