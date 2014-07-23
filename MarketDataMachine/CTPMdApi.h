#pragma once

#include "ctp\ThostFtdcMdApi.h"
#include <qobject.h>
#include <qstring.h>
#include <memory>
#include <vector>

//使用CTP提供的接口实现软件的行情接收api
class CTPMdApi :public QObject{
	Q_OBJECT
public:
	CTPMdApi();
	~CTPMdApi();
	void registerSpi(std::shared_ptr<CThostFtdcMdSpi> spi);					//注册回调接口
	void login();															//登陆
	void release();															//释放接口
	int subscribeData(char *ppInstrumentID[], int nCount);					//订阅行情
	int unsubscribeData(char *ppInstrumentID[], int nCount);				//退订行情
	//返回api的状态
	bool isConnect();
	//设置api的状态，api一旦初始化连接上主机就不再断开，因此一旦初始化连接状态符号就为真且不变
	void setConnect();
	//注册前置机并初始化Api,该函数需要通过线程间接调用，不可以直接调用
	void connectServer();
private:
	CThostFtdcMdApi *ctpMdApi;
	std::shared_ptr<CThostFtdcMdSpi> ctpMdSpi;
	//连接和登陆状态符
	bool connectFlag;
};

