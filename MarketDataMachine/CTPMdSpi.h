#pragma once

#include "ctp\ThostFtdcMdApi.h"
#include "CTPMdApi.h"
#include "Recorder.h"
#include <qobject.h>
#include <qstring.h>
#include <memory>
#include <set>
#include <map>

class CTPMdSpi :public QObject, public CThostFtdcMdSpi{
	Q_OBJECT
public:
	CTPMdSpi(std::shared_ptr<CTPMdApi> api);
	~CTPMdSpi();
	//当客户端与交易后台建立起通信连接时（还未登录前），该方法被调用
	void OnFrontConnected() override;
	
	///登录请求响应
	void OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) override;

	//行情回报响应
	void OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData) override;

	//取消订阅行情应答
	void OnRspUnSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument,
									CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) override;

	////返回正在订阅的合约集合
	//std::set<QString> & getInstru();

	//释放资源
	void release();
private:
	//api退订行情后更新spi
	void updateSetMap(QString instruName);
private:
	std::shared_ptr<CTPMdApi> ctpMdApi;
	std::map<QString, std::shared_ptr<Recorder>> recorderMapping;	//根据合约名字快速定位到记录器
signals:
	void loginSuccess();	//登陆成功信号
};
