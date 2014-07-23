#include "DBwriter.h"
#include "GVAR.h"
#include <qmessagebox.h>
#include <qsqlquery.h>
#include <qfile.h>
#include <qtextstream.h>
#include <qdebug.h>

#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif

DBwriter* DBwriter::writer = nullptr;

//单例模式
DBwriter *DBwriter::getInstance(){
	if (writer == nullptr){
		writer = new DBwriter();
		return writer;
	}
	else
	{
		return writer;
	}
}

DBwriter::DBwriter(){
	db = QSqlDatabase::addDatabase(DB_DRIVER_NAME);
	db.setHostName(DB_HOST_NAME);
	db.setDatabaseName(DATABASE_NAME);
	db.setUserName(USER_NAME);
	db.setPassword(PASSWORD);
	if (!db.open()){
		QMessageBox::information(0, "数据库错误", "无法打开数据库");
		abort();
	}
	//获得数据库中的表名
	QSqlQuery query(db);
	query.exec("show tables");
	while (query.next()){
		QString tableName = query.value(0).toString();
		tables.insert(tableName);
	}
}

//写入数据库库当中
void DBwriter::processFile(QString filename){
	QFile file(filename);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
		QMessageBox::information(0, "错误", "打开" + filename + "时发生错误");
		abort();
	}
	//获得纯粹的文件名
	QStringList list = filename.split("/");
	QString pureFileName = list.at(list.size() - 1);
	//从文件名中解析得到数据库表的名字和交易日期
	QString date = "";
	int i = 0;
	for (; i < 8; i++){
		date.append(pureFileName[i]);
	}
	QString tableName = "";
	for (; i < pureFileName.length(); i++){
		if (pureFileName[i] == '.')
			break;
		tableName.append(pureFileName[i]);
	}
	//检查数据库中是否有这张表，如果没有弹出错误,一个合约对应两张表
	if (tables.find(tableName) == tables.end()){
		QMessageBox::information(0, "错误", "数据库中没有" + tableName + "表");
		abort();
	}
	QString tableName_day = tableName + "_day";
	if (tables.find(tableName_day) == tables.end()){
		QMessageBox::information(0, "错误", "数据库中没有" + tableName_day + "表");
		abort();
	}
	//检查通过开始写入数据
	QTextStream in(&file);
	//以cu1409为例
	//写入cu1409表的语句
	QSqlQuery insert;
	insert.prepare("insert into " + tableName + " values (?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)");
	//写入cu1409_day的语句
	QSqlQuery insert_day;
	insert_day.prepare("insert into " + tableName_day + " values (?,?,?,?,?,?,?)");
	QString line;
	QStringList l;
	while (!in.atEnd()){
		line = in.readLine();
		l = line.split(",");
		//把数据转换出来
		QString t = l.at(0);
		int ms = l.at(1).toInt();
		double LastPrice = l.at(2).toDouble();
		double HighestPrice = l.at(3).toDouble();
		double LowestPrice = l.at(4).toDouble();
		double BidPrice1 = l.at(5).toDouble();
		int BidVolume1 = l.at(6).toInt();
		double AskPrice1 = l.at(7).toDouble();
		int AskVolume1 = l.at(8).toInt();
		int Volume = l.at(9).toInt();
		int OpenInterest = l.at(10).toInt();
		double Turnover = l.at(11).toDouble();
		double ClosePrice = l.at(12).toDouble();
		double AveragePrice = l.at(13).toDouble();
		//绑定查询
		insert.bindValue(0, date);
		insert.bindValue(1, t);
		insert.bindValue(2, ms);
		insert.bindValue(3, LastPrice);
		insert.bindValue(4, HighestPrice);
		insert.bindValue(5, LowestPrice);
		insert.bindValue(6, BidPrice1);
		insert.bindValue(7, BidVolume1);
		insert.bindValue(8, AskPrice1);
		insert.bindValue(9, AskVolume1);
		insert.bindValue(10, Volume);
		insert.bindValue(11, OpenInterest);
		insert.bindValue(12, Turnover);
		insert.bindValue(13, ClosePrice);
		insert.bindValue(14, AveragePrice);
		//执行语句
		bool result = insert.exec();
		qDebug() << result;
	}
	/*******************只需写一次的***********************/
	double OpenPrice = l.at(14).toDouble();
	double UpperLimitPrice = l.at(15).toDouble();
	double LowerLimitPrice = l.at(16).toDouble();
	double PreClosePrice = l.at(17).toDouble();
	double PreOpenInterest = l.at(18).toDouble();
	double PreSettlementPrice = l.at(19).toDouble();
	insert_day.bindValue(0, date);
	insert_day.bindValue(1, OpenPrice);
	insert_day.bindValue(2, UpperLimitPrice);
	insert_day.bindValue(3, LowerLimitPrice);
	insert_day.bindValue(4, PreClosePrice);
	insert_day.bindValue(5, PreOpenInterest);
	insert_day.bindValue(6, PreSettlementPrice);
	bool result2 = insert_day.exec();
	qDebug() << result2;

	//写完以后把这份表格存入log日志中
	QFile logFile("log/persistence.log");
	if (!logFile.open(QIODevice::ReadWrite | QIODevice::Text | QIODevice::Append)){
		QMessageBox::information(0, "错误", "打开persistence.log时发生错误");
		abort();
	}
	QTextStream out(&logFile);
	out << filename << "\n";
	out.flush();
	logFile.close();
	file.close();
}