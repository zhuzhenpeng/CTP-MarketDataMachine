#include "UnsubscribeController.h"
#include <qstringlist.h>
using std::vector;
using std::shared_ptr;

UnsubscribeController::UnsubscribeController() = default;

void UnsubscribeController::setTextLine(QLineEdit* s){
	source = s;
}

void UnsubscribeController::setReceiver(shared_ptr<Receiver> r){
	receiver = r;
}

void UnsubscribeController::reset(){
	receiver = nullptr;
	source = nullptr;
}

void UnsubscribeController::unsubscribe(){
	//从文本框获取输入
	QString &instruments = source->text();
	//根据分号进行分割得到每一个合约代码
	QStringList list = instruments.split(";", QString::SkipEmptyParts);
	vector<QString> v;
	//把一些输入过滤掉：空的输入，首字符不是字母的输入，输入长度大于6的输入
	for (int i = 0; i < list.size(); i++){
		QString item = list.at(i).trimmed();
		if (item != "" && item[0].isLetter() && item.length() <= 6){
			v.push_back(item);
		}
	}
	//创建原始的数组，把数据从vector中转到数组中
	int count = v.size();
	char **ppInstrumentID = new char *[count];
	for (int i = 0; i < count; i++){
		ppInstrumentID[i] = new char[7];
		strcpy(ppInstrumentID[i], v[i].toStdString().c_str());
	}
	receiver->unsubscribeData(ppInstrumentID, count);
	//清空输入框
	source->clear();
}