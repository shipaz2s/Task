#include <QApplication>
#include "mainwnd.hpp"

int main(int argc, char* argv[])
{
	QApplication app(argc, argv);
	MainWnd wnd;
	wnd.show();

 	return app.exec();
}