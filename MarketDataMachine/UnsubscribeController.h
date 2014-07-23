#pragma once

#include <qobject.h>
#include <memory>
#include <qlineedit.h>
#include "Receiver.h"

class UnsubscribeController :public QObject{
	Q_OBJECT
public:
	UnsubscribeController();
	//设置与之相关的文本文本输入框
	void setTextLine(QLineEdit* s);
	//设置接收器
	void setReceiver(std::shared_ptr<Receiver> r);
	//恢复初始状态
	void reset();
public slots:
	void unsubscribe();
private:
	std::shared_ptr<Receiver> receiver;
	QLineEdit* source;
};