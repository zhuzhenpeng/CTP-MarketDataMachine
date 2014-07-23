#include "MainWindow.h"
#include "GVAR.h"
#include "MdTable.h"
#include <qboxlayout.h>
#include <qdatetime.h>
#include <qstring.h>
#include <qstatusbar.h>
#include <qdebug.h>
#include <qmessagebox.h>
#include <qmenubar.h>
using std::shared_ptr;
using std::make_shared;


#include <qtablewidget.h>

#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif

MainWindow::MainWindow(QWidget *parent) :QMainWindow(parent){

	//初始化连接、断开按钮
	connectButton = new QPushButton("连接主机");
	disconnectButton = new QPushButton("断开主机");
	connect(disconnectButton, SIGNAL(clicked()), this, SLOT(disconnect()));
	disconnectButton->setEnabled(false);
	//初始化订阅、退订按键
	subscribeInput = new QLineEdit();
	subscribeInput->setEnabled(false);
	subscribeButton = new QPushButton("订阅行情");
	subscribeButton->setEnabled(false);				//初始化时按钮是不可按的
	unsubscribeInput = new QLineEdit();
	unsubscribeInput->setEnabled(false);
	unsubscribeButton = new QPushButton("退订行情");
	unsubscribeButton->setEnabled(false);
	// 订阅/退订行情区域
	//最上面两“层”的布局
	QHBoxLayout *top1 = new QHBoxLayout();
	top1->addWidget(new QLabel("合约代码:"));
	subscribeInput->setFixedSize(600, 20);
	top1->addWidget(subscribeInput);
	top1->addWidget(subscribeButton);
	QHBoxLayout *top2 = new QHBoxLayout();
	top2->addWidget(new QLabel("合约代码:"));
	unsubscribeInput->setFixedSize(600, 20);
	top2->addWidget(unsubscribeInput);
	top2->addWidget(unsubscribeButton);
	QVBoxLayout *top = new QVBoxLayout();
	top->addLayout(top1);
	top->addLayout(top2);

	//设置表格
	table = new MdTable();
	QVBoxLayout *conLayout = new QVBoxLayout();
	conLayout->addStretch();
	conLayout->addWidget(connectButton);
	conLayout->addWidget(disconnectButton);
	QHBoxLayout *down = new QHBoxLayout();		//下方的布局由表格和连接按钮那一块组成
	down->addWidget(table);
	down->addLayout(conLayout);
	down->setAlignment(Qt::AlignTop);

	QVBoxLayout *mainLayout = new QVBoxLayout();
	mainLayout->addLayout(top);
	mainLayout->addLayout(down);

	QWidget *mainW = new QWidget();
	mainW->setLayout(mainLayout);
	setCentralWidget(mainW);

	createStatusBur();	//创建状态栏
	createMenu();		//创建菜单栏
	initController();

	//设置窗口属性
	setWindowTitle("行情主机");
}

/************************************私有函数*******************************************************/

//初始化控制器
void MainWindow::initController(){
	connectController = make_shared<ConnectController>();
	//连接按钮与连接控制器连接
	connect(connectButton, SIGNAL(clicked()), connectController.get(), SLOT(connectServer()));
	//连接控制器与状态栏连接
	connect(connectController.get(), SIGNAL(connecting()), this, SLOT(statusConnecting()));
	//连接控制器通知连接失败
	connect(connectController.get(), SIGNAL(connectFailed()), this, SLOT(statusConnectFailed()));
	//连接控制器通知登陆成功
	connect(connectController.get(), SIGNAL(loginSuccess()), this, SLOT(statusConnectSuccess()));

	unsubController = make_shared<UnsubscribeController>();
	//订阅控制器与按钮连接
	connect(unsubscribeButton, SIGNAL(clicked()), unsubController.get(), SLOT(unsubscribe()));

	subController = make_shared<SubscribeController>();
	//退订控制器与按钮连接
	connect(subscribeButton, SIGNAL(clicked()), subController.get(), SLOT(subscribe()));
}

//创建状态栏
void MainWindow::createStatusBur(){
	//设置时间标签
	timeLabel = new QLabel("");
	timerForUpdateTimeLabel = new QTimer(this);
	connect(timerForUpdateTimeLabel, SIGNAL(timeout()), this, SLOT(updateTime()));
	timerForUpdateTimeLabel->start(1000);
	//初始化状态标签
	statusLabel = new QLabel(OFFLINE);
	statusBar()->addWidget(statusLabel);
	statusBar()->addPermanentWidget(timeLabel);
}

//创建菜单栏
void MainWindow::createMenu(){
	//选择菜单
	selectMenu = menuBar()->addMenu("选择");
	selectShowMd = new QAction("展示合约", this);
	connect(selectShowMd, SIGNAL(triggered()), this, SLOT(showSelectMdWindow()));
	selectMenu->addAction(selectShowMd);
}

/************************************槽函数*********************************************************/

//时刻更新本地时间
void MainWindow::updateTime(){
	QTime time = QTime::currentTime();
	QString timeStr = time.toString("hh:mm:ss");
	timeLabel->setText("本地时间: " + timeStr);
}

//状态变更为正在连接,连接按钮不可用
void MainWindow::statusConnecting(){
	statusLabel->setText(CONNECTING);
	connectButton->setEnabled(false);
}

//连接失败
void MainWindow::statusConnectFailed(){
	statusLabel->setText(OFFLINE);
	connectController->reset();
	QMessageBox::information(this, "连接失败", "请查看前置机地址是否正确");
	connectButton->setEnabled(true);
}

//连接成功后状态变更为在线，使得订阅和退订可执行
void MainWindow::statusConnectSuccess(){
	statusLabel->setText(ONLINE);
	//设置订阅/退订控制器
	subController->setReceiver(connectController->getReceiver());
	subController->setTextLine(subscribeInput);
	unsubController->setReceiver(connectController->getReceiver());
	unsubController->setTextLine(unsubscribeInput);

	//激活相关的窗口部件
	subscribeButton->setEnabled(true);
	subscribeInput->setEnabled(true);
	unsubscribeButton->setEnabled(true);
	unsubscribeInput->setEnabled(true);
	disconnectButton->setEnabled(true);
}


//断开连接,初始化与api相关的控制器
//通知把表格内容清空
void MainWindow::disconnect(){
	//让各控制器恢复原本状态，使得后台api自动销毁
	subController->reset();
	unsubController->reset();
	connectController->reset();
	//禁用相关部件
	subscribeButton->setEnabled(false);
	subscribeInput->clear();
	subscribeInput->setEnabled(false);
	unsubscribeButton->setEnabled(false);
	unsubscribeInput->clear();
	unsubscribeInput->setEnabled(false);
	disconnectButton->setEnabled(false);
	//让连接控制器可用
	connectButton->setEnabled(true);
	//修改状态栏显示
	statusLabel->setText(OFFLINE);
	table->resetTable();
}

//展示对话框让使用者选择显示的合约
void MainWindow::showSelectMdWindow(){
	if (sMdWindow == nullptr){
		sMdWindow = new SelectShowMdWindow();
		connect(sMdWindow, SIGNAL(showChange(std::set<QString>&)), table, SLOT(updateTableForSelect(std::set<QString>&)));
		connect(unsubscribeButton, SIGNAL(clicked()), table, SLOT(callUnsubUpdateTimer()));
	}
	sMdWindow->showDialog();
}