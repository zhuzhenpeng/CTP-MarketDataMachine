#include "CTPMdSpi.h"
#include "ctp\ThostFtdcUserApiStruct.h"
#include "GVAR.h"
#include <qdebug.h>
#include <qdatetime.h>
#include <mutex>
using std::shared_ptr;
using std::make_shared;
using std::set;
using std::map;
using std::make_pair;
using std::once_flag;

//记录交易日的地方只执行一次
once_flag ONCEFLAG;


#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif

CTPMdSpi::CTPMdSpi(shared_ptr<CTPMdApi> api){
	ctpMdApi = api;
}

CTPMdSpi::~CTPMdSpi(){
	qDebug() << "~CTPMdSpi";
}

//当客户端与交易后台建立起通信连接时（还未登录前），该方法被调用
void CTPMdSpi::OnFrontConnected(){
	ctpMdApi->setConnect();
	ctpMdApi->login();		//连接成功后马上登陆
}

///登录请求响应
void CTPMdSpi::OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin,
	CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast){
	if (pRspInfo->ErrorID == 0){
		qDebug() << "登录成功";
		emit loginSuccess();	//发送登录成功信号
	}
	else{
		qDebug() << "登录失败:" << pRspInfo->ErrorID << " " << pRspInfo->ErrorMsg;
	}

}

//行情回报响应
void CTPMdSpi::OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData){
	QString &td = tradeDate;
	std::call_once(ONCEFLAG, [&td, &pDepthMarketData](){
		//获取当前交易日
		td = QString(pDepthMarketData->TradingDay);
	});
	//合约名字是合约代号 (如：cu1409)
	QString instruName(pDepthMarketData->InstrumentID);
	if (instruSet.find(instruName) == instruSet.end()){	//不存在于集合当中
		//说明收到了某订阅合约的数据，为该合约初始化记录器
		auto recorder = make_shared<Recorder>(instruName);
		recorder->record(pDepthMarketData);	//记录行情
		instruSet.insert(instruName);		//放入set容器中
		recorderMapping.insert(make_pair(instruName, recorder));	//放入map容器中
	}
	else{
		recorderMapping[instruName]->record(pDepthMarketData);	//快速找到记录器并且把行情记录
	}
	//更新合约信息
	QString code(pDepthMarketData->InstrumentID);
	instruList[code]->update(pDepthMarketData);
}

//取消订阅行情应答
void CTPMdSpi::OnRspUnSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument,
	CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast){
	if (pRspInfo->ErrorID == 0){
		QString instruName(pSpecificInstrument->InstrumentID);
		updateSetMap(instruName);
		qDebug() << "退订行情响应";
	}
}

//api退订行情后更新spi
void CTPMdSpi::updateSetMap(QString instruName){
	auto iter = instruSet.find(instruName);
	if (iter == instruSet.end()){
		return;
	}
	else{
		//退订合约后不会再收到合约的信息，因此记录合约的对象销毁
		//把合约从set和map当中去除
		instruSet.erase(instruName);
		recorderMapping.erase(instruName);
	}
}

//释放资源,必须先在api中对spi解引用
void CTPMdSpi::release(){
	ctpMdApi = nullptr;
	for (auto iter = recorderMapping.begin(); iter != recorderMapping.end(); iter++){
		(*iter).second->release();
	}
}