#include "Recorder.h"
#include "GVAR.h"
#include <qdebug.h>

//记录器的名字就是合约的名字，文件名命名方式为日期加上合约名字，如20140718cu1408
Recorder::Recorder(QString name){
	recorderName = tradeDate + name;
	file.setFileName(DAT_PREDIR + recorderName + ".csv");
	//总是以追加的方式打开文件
	if (!file.open(QIODevice::ReadWrite | QIODevice::Text | QIODevice::Append)){
		qDebug() << file.fileName() << " open fail";
		abort();
	}
	recorder.setDevice(&file);
}

Recorder::~Recorder(){
	qDebug() << "~Recorder";
}

//记录行情
void Recorder::record(CThostFtdcDepthMarketDataField *data){
	if (recorder.status() == QTextStream::Ok){
		recorder << data->UpdateTime << ","			//最后修改时间
			<< data->UpdateMillisec << ","			//最后修改毫秒
			<< data->LastPrice << ","				//最新价
			<< data->HighestPrice << ","				//最高价
			<< data->LowestPrice << ","				//最低价
			<< data->BidPrice1 << ","				//申买价一
			<< data->BidVolume1 << ","				//申买量一
			<< data->AskPrice1 << ","				//申卖价一
			<< data->AskVolume1 << ","				//申卖量一
			<< data->Volume << ","					//当前成交量
			<< data->OpenInterest << ","				//持仓量
			<< data->Turnover << ","					//成交金额
			<< data->ClosePrice << ","				//今日收盘价
			<< data->AveragePrice << ","			//今日均价
	/****************上面的数据每个时刻都不同，下面的每一天不同**************************/
			<< data->OpenPrice << ","				//今日开盘价
			<< data->UpperLimitPrice << ","			//涨停价
			<< data->LowerLimitPrice << ","			//跌停价
			<< data->PreClosePrice << ","			//昨收盘价
			<< data->PreOpenInterest << ","			//昨持仓量
			<< data->PreSettlementPrice << "\n";		//昨结算价

		recorder.flush();
	}
	else{
		qDebug() << file.fileName() << " recorder error!!";
	}
}

//释放资源
void Recorder::release(){
	file.close();
}