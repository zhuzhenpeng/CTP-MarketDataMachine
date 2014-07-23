#include "SelectShowMdWindow.h"
#include "GVAR.h"
#include <cmath>
#include <qmessagebox.h>
#include <qlayout.h>
#include <qdebug.h>

#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif

SelectShowMdWindow::SelectShowMdWindow(){
	setWindowTitle("选择要显示的合约");
	QVBoxLayout *mainLayout = new QVBoxLayout();
	//遍历合约列表,初始化复选框
	for (auto iter = instruList.begin(); iter != instruList.end(); iter++){
		QCheckBox *checkItem = new QCheckBox((*iter).first, this);	//设置每个复选框的名字为合约名字
		checkItem->setEnabled(false);
		instruCheckBox.push_back(checkItem);
	}
	//20个复选框放在一行，再把每行放在一起
	int layoutNum = ceil(instruCheckBox.size()/5.0);
	QHBoxLayout **hLayouts = new QHBoxLayout*[layoutNum];
	//初始化每一个行布局并把复选框放入
	for (int i = 0; i < layoutNum; i++){
		hLayouts[i] = new QHBoxLayout;
	}
	int rowNum = 0;	// rowNum/20得到行数,rowNum递增
	for (auto iter = instruCheckBox.begin(); iter != instruCheckBox.end(); iter++){
		int row = rowNum / 20;
		hLayouts[row]->addWidget((*iter));
		rowNum++;
	}
	//把每个行布局放入总布局
	for (int i = 0; i < layoutNum; i++){
		mainLayout->addLayout(hLayouts[i]);
	}
	//初始化按钮
	okButton = new QPushButton("确定");
	cancelButton = new QPushButton("取消");
	QHBoxLayout *buttonLayout = new QHBoxLayout();
	buttonLayout->addWidget(okButton);
	buttonLayout->addWidget(cancelButton);
	mainLayout->addLayout(buttonLayout);
	connect(cancelButton, SIGNAL(clicked()), this, SLOT(hide()));
	connect(okButton, SIGNAL(clicked()), this, SLOT(pushOkButton()));
	setLayout(mainLayout);
	setModal(true);	//设置为模态
	hide();
}

//显示该窗口，用户选择显示合约时发生
void SelectShowMdWindow::showDialog(){
	if (instruSet.empty()){
		QMessageBox::information(this, " ", "请先选择合约进行行情接收");
		hide();
	}
	else
	{
		//1.把全部选框初始化为不可选且没选
		//2.把已经订阅的行情的复选框设置为可选
		//3.把之前订阅的合约，在现在可选的复选框上自动选上
		//每个复选框以它的名字作为ID
		for (auto iter = instruCheckBox.begin(); iter != instruCheckBox.end(); iter++){
			(*iter)->setChecked(false);
			(*iter)->setEnabled(false);
			if (instruSet.find((*iter)->text()) != instruSet.end()){
				(*iter)->setEnabled(true);
				if (showedInstru.find((*iter)->text()) != showedInstru.end()){
					(*iter)->setChecked(true);
				}
			}
		}
		show();
	}
}

void SelectShowMdWindow::pushOkButton(){
	showedInstru.clear();
	//复选框选上则放入集合当中
	for (auto iter = instruCheckBox.begin(); iter != instruCheckBox.end(); iter++){
		if ((*iter)->isChecked()){
			showedInstru.insert((*iter)->text());
		}
	}
	emit showChange(showedInstru);
	hide();
}