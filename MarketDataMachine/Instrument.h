#pragma once

#include <qstring.h>
#include "ctp\ThostFtdcMdApi.h"

class Instrument{
public:
	Instrument(QString c, QString n);
	void update(CThostFtdcDepthMarketDataField *p);
public:
	const QString code;
	const QString name;
	double lastPrice;
	double bidPrice;
	int bidVolumn;
	double askPrice;
	int askVolumn;
	double highestPrice;
	double lowestPrice;
	int volumn;
	int openInterest;
};