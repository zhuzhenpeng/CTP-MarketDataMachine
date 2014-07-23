#pragma once

#include <qstring.h>
#include <qfile.h>
#include <qtextstream.h>
#include <qsqldatabase.h>
#include <set>

class DBwriter{
public:
	static DBwriter *getInstance();
	void processFile(QString filename);
private:
	//µ¥ÀýÄ£Ê½
	DBwriter();
private:
	static DBwriter *writer;
	QSqlDatabase db;
	std::set<QString> tables;
};