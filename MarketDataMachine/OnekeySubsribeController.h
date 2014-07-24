#pragma once

#include <qobject.h>
#include <memory>
#include "Receiver.h"

class OnekeySubsribeController :public QObject{
	Q_OBJECT
public:
	OnekeySubsribeController();
	void setReceiver(std::shared_ptr<Receiver> r);
	void reset();
public slots:
	void onekeySubscribe();
private:
	std::shared_ptr<Receiver> receiver;
};