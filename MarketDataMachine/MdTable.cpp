#include "MdTable.h"
#include "GVAR.h"
#include <qdebug.h>
using std::set;
using std::make_pair;

#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif

MdTable::MdTable(){
	this->setColumnCount(7);
	//设置表头内容 
	QStringList header;
	header << "合约代码" << "合约名" << "最新价" << "买价" << "买量" << "卖价" <<
		"卖量";
	this->setHorizontalHeaderLabels(header);
	//设置不可编辑
	this->setEditTriggers(QAbstractItemView::NoEditTriggers);

	//设置定时器
	updateTimer = new QTimer(this);
	connect(updateTimer, SIGNAL(timeout()), this, SLOT(refreshTable()));
	unsubUpdateTimer = new QTimer(this);
	unsubUpdateTimer->setSingleShot(true);
	connect(unsubUpdateTimer, SIGNAL(timeout()), this, SLOT(updateTableForUnsub()));
}

//断开连接后初始化表格
void MdTable::resetTable(){
	updateTimer->stop();
	this->clearContents();	//清除表格内容
	//注意从最后一行往前删除
	int i = instruInTable.size();
	for (; i >= 0; i--){
		this->removeRow(i);
	}
	instruInTable.clear();
}

//刷新表格
void MdTable::refreshTable(){
	int row = 0;
	for (auto iter = instruInTable.begin(); iter != instruInTable.end(); iter++){
		Instrument* i = instruList[(*iter)];
		QTableWidgetItem *third = new QTableWidgetItem();
		third->setText(QString::number(i->lastPrice));
		QTableWidgetItem *forth = new QTableWidgetItem();
		forth->setText(QString::number(i->bidPrice));
		QTableWidgetItem *fifth = new QTableWidgetItem();
		fifth->setText(QString::number(i->bidVolumn));
		QTableWidgetItem *sixth = new QTableWidgetItem();
		sixth->setText(QString::number(i->askPrice));
		QTableWidgetItem *seventh = new QTableWidgetItem();
		seventh->setText(QString::number(i->askVolumn));
		this->setItem(row, 2, third);
		this->setItem(row, 3, forth);
		this->setItem(row, 4, fifth);
		this->setItem(row, 5, sixth);
		this->setItem(row, 6, seventh);
		row++;
	}
}

//因为用户选择而更新列表,参数newInstru由用户在对话框中选择合约后传过来
void MdTable::updateTableForSelect(set<QString>& newInstru){
	updateTimer->stop();   //停止更新表格
	for (auto iter = instruInTable.begin(); iter != instruInTable.end();){
		if (newInstru.find(*iter) == newInstru.end()){	//去掉不要的合约
			iter = instruInTable.erase(iter);
		}
		else
		{
			iter++;
		}
	}
	for (auto iter = newInstru.begin(); iter != newInstru.end(); iter++){
		auto i = instruInTable.begin();
		for (; i != instruInTable.end(); i++){
			if ((*i) == (*iter)){
				break;
			}
		}
		if (i == instruInTable.end()){ //添加新进来的合约
			instruInTable.push_back((*iter));
		}
	}
	qDebug() << "订阅合约列表";
	for (auto iter = newInstru.begin(); iter != newInstru.end(); iter++){
		qDebug() << (*iter);
	}
	qDebug() << "表格合约列表";
	for (auto iter = instruInTable.begin(); iter != instruInTable.end(); iter++){
		qDebug() << (*iter);
	}
	//更新显示的表格
	this->clearContents();
	int rowCount = this->rowCount();
	for (; rowCount >= 0; rowCount--){
		this->removeRow(rowCount);
	}
	int row = 0;
	for (auto iter = instruInTable.begin(); iter != instruInTable.end(); iter++){
		Instrument* i = instruList[(*iter)];
		QTableWidgetItem *first = new QTableWidgetItem();
		first->setText(i->code);
		QTableWidgetItem *second = new QTableWidgetItem();
		second->setText(i->name);
		QTableWidgetItem *third = new QTableWidgetItem();
		third->setText(QString::number(i->lastPrice));
		QTableWidgetItem *forth = new QTableWidgetItem();
		forth->setText(QString::number(i->bidPrice));
		QTableWidgetItem *fifth = new QTableWidgetItem();
		fifth->setText(QString::number(i->bidVolumn));
		QTableWidgetItem *sixth = new QTableWidgetItem();
		sixth->setText(QString::number(i->askPrice));
		QTableWidgetItem *seventh = new QTableWidgetItem();
		seventh->setText(QString::number(i->askVolumn));
		this->insertRow(row);
		this->setItem(row, 0, first);
		this->setItem(row, 1, second);
		this->setItem(row, 2, third);
		this->setItem(row, 3, forth);
		this->setItem(row, 4, fifth);
		this->setItem(row, 5, sixth);
		this->setItem(row, 6, seventh);
		row++;
	}
	this->update();
	updateTimer->start(1000);
}

//退订后过1.5秒更新
void MdTable::callUnsubUpdateTimer(){
	unsubUpdateTimer->start(1500);
}

//因为用户退订而更新列表
void MdTable::updateTableForUnsub(){
	for (auto iter = instruInTable.begin(); iter != instruInTable.end(); ){
		if (instruSet.find(*iter) == instruSet.end()){	//在正在订阅列表中没找到相关合约则去除
			int row = std::distance(instruInTable.begin(), iter);
			this->removeRow(row);
			iter = instruInTable.erase(iter);
		}
		else{
			iter++;
		}
	}
}