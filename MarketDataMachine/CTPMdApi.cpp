#include "GVAR.h"
#include "CTPMdApi.h"
#include <qdebug.h>
#include <string>
using std::string;
using std::shared_ptr;
using std::vector;

#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif


CTPMdApi::CTPMdApi(){
	ctpMdApi = CThostFtdcMdApi::CreateFtdcMdApi("./ctp_temp_file/");
	connectFlag = false;
}

CTPMdApi::~CTPMdApi(){
	qDebug() << "ctpMdApi->Release()";
	if (ctpMdApi != nullptr){
		ctpMdApi->RegisterSpi(NULL);
		ctpMdApi->Release();
		ctpMdApi = NULL;
		instruSet.clear();	//情况已订阅的列表
	}
}

/************************************public函数**************************************************************/

void CTPMdApi::registerSpi(shared_ptr<CThostFtdcMdSpi> spi){
	ctpMdSpi = spi;
	ctpMdApi->RegisterSpi(ctpMdSpi.get());
}

void CTPMdApi::login(){
	CThostFtdcReqUserLoginField loginField;
	strcpy(loginField.BrokerID, "");
	strcpy(loginField.UserID, "");
	strcpy(loginField.Password, "");
	ctpMdApi->ReqUserLogin(&loginField, 0);
}

void CTPMdApi::release(){
	ctpMdApi->Release();
	ctpMdApi = nullptr;
}

int CTPMdApi::subscribeData(char *ppInstrumentID[], int nCount){
	return ctpMdApi->SubscribeMarketData(ppInstrumentID, nCount);
}

int CTPMdApi::unsubscribeData(char *ppInstrumentID[], int nCount){
	return ctpMdApi->UnSubscribeMarketData(ppInstrumentID, nCount);
}

bool CTPMdApi::isConnect(){
	return connectFlag;
}

void CTPMdApi::setConnect(){
	connectFlag = true;
}

//注册前置机并初始化Api
void CTPMdApi::connectServer(){
	//注册前置机
	char address[100];
	strcpy(address, FRONT_ADDRESS.toStdString().c_str());
	ctpMdApi->RegisterFront(address);
	//初始化Api
	ctpMdApi->Init();
}

