#pragma once

//用户在该对话框中选择要显示在行情表单的合约
#include <qobject.h>
#include <qdialog.h>
#include <qstring.h>
#include <qpushbutton.h>
#include <qcheckbox.h>
#include <set>
#include <vector>

class SelectShowMdWindow :public QDialog{
	Q_OBJECT
public:
	SelectShowMdWindow();
	void showDialog();
private:
	std::set<QString> showedInstru;				//在表格中展示的合约代码集合
	std::vector<QCheckBox*>  instruCheckBox;	//全部合约的复选框
	QPushButton *okButton;
	QPushButton *cancelButton;
private slots:
	void pushOkButton();
signals:
	void showChange(std::set<QString>& newInstru);
};