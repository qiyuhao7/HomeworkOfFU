#include "imageframe1.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	ImageFrame1 w;
	w.show();
	return a.exec();
}
