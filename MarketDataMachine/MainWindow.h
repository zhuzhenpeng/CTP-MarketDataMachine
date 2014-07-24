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
#include "OnekeySubsribeController.h"
#include "SubscribingDialog.h"
#include "EditOnekeyDialog.h"

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

	//与showSubscribingAction动作相联的槽函数
	void showSubscribing();			//展示正在订阅的合约的对话框

	//与editOnekeyAction动作相联的槽函数
	void showEditOnekeyDialog();	//展示编辑Onekey配置文件的对话框
private:
	//按钮和输入框
	QPushButton *connectButton;		//连接主机
	QPushButton *disconnectButton;	//断开主机
	QLineEdit *subscribeInput;		//订阅行情输入框
	QPushButton *subscribeButton;	//订阅行情
	QLineEdit *unsubscribeInput;	//退订行情输入框
	QPushButton *unsubscribeButton;	//退订行情
	QPushButton *onekeySubButton;	//一键订阅

	//状态栏的控件及相关
	QLabel *timeLabel;
	QTimer *timerForUpdateTimeLabel;
	QLabel *statusLabel;

	//对话框
	SelectShowMdWindow *sMdWindow=nullptr;
	SubscribingDialog *subDialog = nullptr;
	EditOnekeyDialog *editOnekeyDialog = nullptr;

	//表格
	MdTable *table;

	//菜单栏
	QMenu *selectMenu;
	QMenu *checkMenu;
	QMenu *editMenu;

	//动作
	QAction *selectShowMd;
	QAction *writeToDB;
	QAction *showSubscribingAction;
	QAction *editOnekeyAction;

	//控制器
	std::shared_ptr<ConnectController> connectController;
	std::shared_ptr<SubscribeController> subController;
	std::shared_ptr<UnsubscribeController> unsubController;
	std::shared_ptr<DBWriterController> dbWriterController;
	std::shared_ptr<OnekeySubsribeController> onekeySubController;
};