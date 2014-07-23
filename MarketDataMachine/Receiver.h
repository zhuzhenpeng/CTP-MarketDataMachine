#pragma once

#include <qobject.h>
#include <qtimer.h>
#include <memory>
#include "CTPMdApi.h"
#include "CTPMdSpi.h"

class Receiver :public QObject{
	Q_OBJECT
public:
	Receiver();
	~Receiver();
	//初始化线程，让线程对象发出信号通知api连接主机
	void connectServer();
	//订阅行情
	int subscribeData(char *ppInstrumentID[], int nCount);
	//退订行情
	int unsubscribeData(char *ppInstrumentID[], int nCount);
private:
	std::shared_ptr<CTPMdApi> api;
	std::shared_ptr<CTPMdSpi> spi;
	QTimer *connectOverTimer;	//连接超时计时器
signals:
	//与控制器相关的信号
	void connectFailed();
	void loginSuccess();
private slots:
	//配合连接超时计时器检测是否连接超时
	void detectOvertime();
};