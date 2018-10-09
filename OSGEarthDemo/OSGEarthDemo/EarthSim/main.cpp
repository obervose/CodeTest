#include "EarthSimWnd.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	EarthSimWnd w;
	w.show();
	return a.exec();
}
