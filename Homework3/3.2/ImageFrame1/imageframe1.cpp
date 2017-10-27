#include "imageframe1.h"
#include <QtWidgets>
#include <qpainter.h>
#include <imagewidget.h>

ImageFrame1::ImageFrame1(QWidget *parent)
	: QMainWindow(parent)
{
	//ui.setupUi(this);

	setGeometry(300, 150, 800, 450);
	setWindowTitle("Image Wrapping");
	imagewidget_ = new ImageWidget();
	setCentralWidget(imagewidget_);

	CreateActions();
	CreateMenus();
	CreateToolBars();
	CreateStatusBar();
	
}

ImageFrame1::~ImageFrame1()
{

}

void ImageFrame1::CreateActions()
{
	
	action_new_ = new QAction(QApplication::style()->standardIcon(QStyle::SP_FileDialogNewFolder), tr("&New"), this);
	action_new_->setStatusTip(tr("Create a new file"));
	

	action_open_ = new QAction(QApplication::style()->standardIcon(QStyle::SP_DirOpenIcon), tr("&Open..."), this);
	action_open_->setShortcuts(QKeySequence::Open);
	action_open_->setStatusTip(tr("Open an existing file"));
	connect(action_open_, SIGNAL(triggered()), imagewidget_, SLOT(Open()));

	action_save_ = new QAction(QApplication::style()->standardIcon(QStyle::SP_DialogSaveButton), tr("&Save"), this);
	action_save_->setShortcuts(QKeySequence::Save);
	action_save_->setStatusTip(tr("Save the document to disk"));

	action_saveas_ = new QAction(QApplication::style()->standardIcon(QStyle::SP_DialogSaveButton), tr("&SaveAs"), this);
	action_saveas_->setShortcuts(QKeySequence::SaveAs);
	action_saveas_->setStatusTip(tr("Save the document under a new name"));
	connect(action_saveas_, SIGNAL(triggered()), imagewidget_, SLOT(SaveAs()));

	action_invert_ = new QAction(tr("Inverse"), this);
	action_invert_->setStatusTip(tr("Invert all pixel value in the image"));
	connect(action_invert_, SIGNAL(triggered()), imagewidget_, SLOT(Invert()));

	action_restore_ = new QAction(tr("Restore"), this);
	action_restore_->setStatusTip(tr("Show origin image"));
	connect(action_restore_, SIGNAL(triggered()), imagewidget_, SLOT(Restore()));

	action_draw_ = new QAction(tr("Draw Lines"), this);
	action_draw_->setStatusTip(tr("Draw Lines"));
	connect(action_draw_, SIGNAL(triggered()), imagewidget_, SLOT(Draw()));

	action_idw_draw_ = new QAction(tr("IDWDraw"), this);
	action_idw_draw_->setStatusTip(tr("IDWDraw"));
	connect(action_idw_draw_, SIGNAL(triggered()), imagewidget_, SLOT(IDWDraw()));

	action_rbf_draw_ = new QAction(tr("RBFDraw"), this);
	action_rbf_draw_->setStatusTip(tr("RBFDraw"));
	connect(action_rbf_draw_, SIGNAL(triggered()), imagewidget_, SLOT(RBFDraw()));

}

void ImageFrame1::CreateMenus()
{
	menu_file_ = menuBar()->addMenu(tr("&File"));
	menu_file_->setStatusTip(tr("File menu"));
	//menu_file_->addAction(action_new_);
	menu_file_->addAction(action_open_);
	//menu_file_->addAction(action_save_);
	menu_file_->addAction(action_saveas_);

	menu_edit_ = menuBar()->addMenu(tr("&Edit"));
	menu_edit_->setStatusTip(tr("Edit menu"));
	//menu_edit_->addAction(action_invert_);
	//menu_edit_->addAction(action_restore_);
	menu_edit_->addAction(action_idw_draw_);
	menu_edit_->addAction(action_rbf_draw_);

}

void ImageFrame1::CreateToolBars()
{
	toolbar_file_ = addToolBar(tr("File"));
	//toolbar_file_->addAction(action_new_);
	toolbar_file_->addAction(action_open_);
	toolbar_file_->addAction(action_saveas_);

	// Add separator in toolbar 
	toolbar_file_->addSeparator();
	
	toolbar_file_->addAction(action_draw_);
	toolbar_file_->addAction(action_idw_draw_);
	toolbar_file_->addAction(action_rbf_draw_);
	//toolbar_file_->addAction(action_invert_);
	toolbar_file_->addAction(action_restore_);
}

void ImageFrame1::CreateStatusBar()
{
	statusBar()->showMessage(tr("Ready"));
}

void ImageFrame1::closeEvent(QCloseEvent *e)
{

}
