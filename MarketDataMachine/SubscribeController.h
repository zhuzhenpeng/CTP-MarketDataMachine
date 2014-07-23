#pragma once

#include <qobject.h>
#include <qstring.h>
#include <qlineedit.h>
#include <memory>
#include "Receiver.h"

class SubscribeController :public QObject{
	Q_OBJECT
public:
	SubscribeController();
	//设置与之相关的文本文本输入框
	void setTextLine(QLineEdit* s);
	//设置接收器
	void setReceiver(std::shared_ptr<Receiver> r);
	//恢复初始状态
	void reset();
public slots:
	void subscribe();
private:
	std::shared_ptr<Receiver> receiver;
	QLineEdit* source;
};
