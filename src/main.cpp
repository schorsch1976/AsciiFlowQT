#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	Q_INIT_RESOURCE(resource);

	a.setWindowIcon(QIcon(":/ico/class.png"));

	MainWindow w;
	w.show();

	return a.exec();
}
