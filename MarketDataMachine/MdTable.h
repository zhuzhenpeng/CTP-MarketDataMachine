#pragma once

#include <qobject.h>
#include <qtablewidget.h>
#include <qtimer.h>
#include <vector>
#include <map>
#include <set>
#include <qstring.h>

class MdTable :public QTableWidget{
	Q_OBJECT
public:
	MdTable();
	void resetTable();		//断开连接后初始化表格
private:
	QTimer *updateTimer;			//自动刷新计时器
	QTimer *unsubUpdateTimer;		//退订更新计时器
	std::vector<QString> instruInTable;		//在table中显示的合约,行号与vector中的插入顺序相同
private slots:
	void refreshTable();	//刷新列表
	void updateTableForUnsub();									//因为用户退订而更新列表
public slots:
	void updateTableForSelect(std::set<QString>& newInstru);	//因为用户选择而更新列表
	void callUnsubUpdateTimer();								//退订行情后通知退订更新计时器
};