#pragma once

//主窗口类
#include <qobject.h>
#include <qmainwindow.h>
#include <qpushbutton.h>
#include <qlabel.h>
#include <qtimer.h>
#include <qlineedit.h>
#include <memory>
#include <qaction.h>
#include <qmenu.h>
#include "ConnectController.h"
#include "SubscribeController.h"
#include "UnsubscribeController.h"
#include "MdTable.h"
#include "SelectShowMdWindow.h"
#include "DBWriterController.h"

class MainWindow :public QMainWindow{
	Q_OBJECT
public:
	MainWindow(QWidget *parent = 0);
private:
	void createStatusBur();
	void createMenu();
	//初始化连接控制器
	void initController();
private slots:
	//与timerForUpdateTimeLabel相联的槽函数
	void updateTime();				//时刻更新本地时间

	//与ConnectController相联的槽函数
	void statusConnecting();		//状态变更为正在连接,连接按钮不可用
	void statusConnectFailed();		//连接失败
	void statusConnectSuccess();	//连接成功后状态变更为在线，使得订阅和退订可执行

	//与disconnectButton相联的槽函数
	void disconnect();				//断开连接,初始化与api相关的控制器
									//通知把表格内容清空

	//与selectShowMd动作相联的槽函数
	void showSelectMdWindow();		//展示对话框让使用者选择显示的合约

	//与writeToDB动作相联的槽函数
	void selectFileForWriter();		//展示对话框让使用者选择文件写入数据库
private:
	//选择订阅和退订的行情区域的控件及相关
	QPushButton *connectButton;
	QPushButton *disconnectButton;
	QLineEdit *subscribeInput;
	QPushButton *subscribeButton;
	QLineEdit *unsubscribeInput;
	QPushButton *unsubscribeButton;

	//状态栏的控件及相关
	QLabel *timeLabel;
	QTimer *timerForUpdateTimeLabel;
	QLabel *statusLabel;

	//对话框
	SelectShowMdWindow *sMdWindow=nullptr;

	//表格
	MdTable *table;

	//菜单栏
	QMenu *selectMenu;

	//动作
	QAction *selectShowMd;
	QAction *writeToDB;

	//控制器
	std::shared_ptr<ConnectController> connectController;
	std::shared_ptr<SubscribeController> subController;
	std::shared_ptr<UnsubscribeController> unsubController;
	std::shared_ptr<DBWriterController> dbWriterController;
};