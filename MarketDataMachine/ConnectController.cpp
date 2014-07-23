#include "ConnectController.h"
using std::shared_ptr;
using std::make_shared;

ConnectController::ConnectController() = default;

//连接控制器初始化自己
void ConnectController::reset(){
	receiver = nullptr;
}

//返回连接控制器中对订阅器的引用
shared_ptr<Receiver> ConnectController::getReceiver(){
	return receiver;
}

/*************************************槽函数***********************************************/

void ConnectController::connectServer(){
	receiver = make_shared<Receiver>();
	//连接两者信号，达到转发Receiver的信号给界面的目的
	connect(receiver.get(), SIGNAL(connectFailed()), this, SIGNAL(connectFailed()));
	connect(receiver.get(), SIGNAL(loginSuccess()), this, SIGNAL(loginSuccess()));
	receiver->connectServer();
	emit connecting();		//通知窗口部件，现在的状态为正在连接
}

