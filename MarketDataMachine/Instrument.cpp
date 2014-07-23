#include "Instrument.h"

Instrument::Instrument(QString c, QString n) :code(c), name(n){
	lastPrice = 0;
	bidPrice = 0;
	bidVolumn = 0;
	askPrice = 0;
	askVolumn = 0;
	highestPrice = 0;
	lowestPrice = 0;
	volumn = 0;
	openInterest = 0;
}

void Instrument::update(CThostFtdcDepthMarketDataField *p){
	lastPrice = p->LastPrice;
	bidPrice = p->BidPrice1;
	bidVolumn = p->BidVolume1;
	askPrice = p->AskPrice1;
	askVolumn = p->AskVolume1;
	highestPrice = p->HighestPrice;
	lowestPrice = p->LowerLimitPrice;
	volumn = p->Volume;
	openInterest = p->OpenInterest;
}