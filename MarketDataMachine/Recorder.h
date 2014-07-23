#pragma once

#include <qstring.h>
#include <qfile.h>
#include <qtextstream.h>
#include "ctp\ThostFtdcUserApiStruct.h"

class Recorder{
public:
	Recorder(QString name);
	~Recorder();
	//记录行情
	void record(CThostFtdcDepthMarketDataField *data);
	//释放资源
	void release();
private:
	QString recorderName;
	QFile file;
	QTextStream recorder;
};

