#include "minidraw1.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	MiniDraw1 w;
	w.show();
	return a.exec();
}
