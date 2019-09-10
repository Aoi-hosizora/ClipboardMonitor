#include "MainDialog.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[]) {
	QApplication a(argc, argv);
	MainDialog w;
	w.show();
	return a.exec();
}
