#pragma once

#include <qobject.h>
#include <memory>
#include <qpushbutton.h>
#include "Receiver.h"

class ConnectController :public QObject{
	Q_OBJECT
public:
	ConnectController();
	//连接控制器初始化自己:用于两种情况1.连接失败，2.断开连接
	void reset();
	std::shared_ptr<Receiver> getReceiver();
	bool isConnect();
private:
	std::shared_ptr<Receiver> receiver;
public slots:
	void connectServer();
signals:
	//通知窗口部件的信号
	void connecting();		//正在连接
	void connectFailed();	//连接失败
	void loginSuccess();	//登陆成功
};
