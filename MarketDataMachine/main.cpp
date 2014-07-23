//#define UNIT_TEST
#ifndef UNIT_TEST
#include <QApplication>
#include "MainWindow.h"
#include "initialize.h"

#pragma comment(lib,"thostmduserapi.lib")

int main(int argc, char *argv[]){
	QApplication a(argc, argv);

	iniFrontAdress();
	iniInstrument();
	MainWindow *mw = new MainWindow();
	mw->show();

	return a.exec();
}

#endif
