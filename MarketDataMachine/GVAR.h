#pragma once

#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif

#include <qstring.h>
#include <map>
#include <set>
#include "Instrument.h"

//前置机地址
extern QString FRONT_ADDRESS;

//行情机状态
extern const QString OFFLINE;
extern const QString CONNECTING;
extern const QString ONLINE;

//放置行情数据文件*.dat的前缀地址
extern const QString DAT_PREDIR;

//全部合约的列表(合约代码)
extern std::map<QString, Instrument*> instruList;

//正在订阅的合约集合(合约代码)
extern std::set<QString> instruSet;

//交易日
extern QString tradeDate;