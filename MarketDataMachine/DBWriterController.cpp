#include "DBWriterController.h"
#include "DBwriter.h"
#include <qmessagebox.h>
#include <qdebug.h>
#include <qfile.h>
#include <qtextstream.h>
#include <QProgressDialog>
#include <set>
using std::set;

#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif

void DBWriterController::readyToWrite(QStringList filenames){
	//查看那日志文件检查选择的文件当中有没有已经写入过数据库当中
	QFile log("log/persistence.log");
	if (!log.open(QIODevice::ReadOnly | QIODevice::Text)){
		QMessageBox::information(0, "错误", "找不到日志文件");
		abort();
	}
	set<QString> wrote;	//记录有哪些文件已经写入过数据库
	QTextStream in(&log);
	while (!in.atEnd()){
		QString text = in.readLine();
		wrote.insert(text);
	}
	QString warning = "";
	for (int i = 0; i < filenames.size(); i++){
		if (wrote.find(filenames.at(i)) != wrote.end()){
			warning.append(filenames.at(i) + "\n");
		}
	}
	if (warning != ""){
		//有重复记入数据库的文件，拒绝写入数据库，要求重新选择
		QMessageBox::information(0, "错误", warning + "已经存入数据库,请重选");
		return;
	}
	//开始处理
	DBwriter *writer = DBwriter::getInstance();
	int fileNum = filenames.size();
	for (int i = 0; i < fileNum; i++) {
		writer->processFile(filenames.at(i));
	}
	QMessageBox::information(0, "完成", "已存入数据库");
}