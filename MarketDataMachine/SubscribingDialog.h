#pragma once

#include <qdialog.h>
#include <qtextedit.h>
#include <qpushbutton.h>

class SubscribingDialog :public QDialog{
	Q_OBJECT
public:
	SubscribingDialog();
	void showDialog();
private:
	QTextEdit *textArea;
	QPushButton *okButton;
};