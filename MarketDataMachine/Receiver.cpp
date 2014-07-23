#include "Receiver.h"
#include <qdebug.h>
using std::shared_ptr;
using std::make_shared;
using std::set;


Receiver::Receiver(){
	api = make_shared<CTPMdApi>();
	spi = make_shared<CTPMdSpi>(api);
	api->registerSpi(spi);
	//api调用函数后,spi反馈结果信号，Receiver传递信号给控制器
	connect(spi.get(), SIGNAL(loginSuccess()), this, SIGNAL(loginSuccess()));
	//设置连接超时计时器
	connectOverTimer = new QTimer(this);
	connect(connectOverTimer, SIGNAL(timeout()), this, SLOT(detectOvertime()));
	connectOverTimer->setSingleShot(true);
}

Receiver::~Receiver(){
	//先让api对Spi解引用
	api->registerSpi(NULL);
	spi->release();
	qDebug() << "~Receiver";
}

//让api连接主机
void Receiver::connectServer(){
	api->connectServer();
	connectOverTimer->start(3 * 1000);
}

//订阅行情
int Receiver::subscribeData(char *ppInstrumentID[], int nCount){
	return api->subscribeData(ppInstrumentID, nCount);
}

//退订行情
int Receiver::unsubscribeData(char *ppInstrumentID[], int nCount){
	return api->unsubscribeData(ppInstrumentID, nCount);
}


//配合连接超时计时器检测是否连接超时
void Receiver::detectOvertime(){
	bool isCon = api->isConnect();
	if (isCon == false){
		emit connectFailed();
	}
}
