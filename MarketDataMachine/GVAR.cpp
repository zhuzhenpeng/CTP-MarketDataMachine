#include "GVAR.h"

//前置机地址
QString FRONT_ADDRESS("");

//行情机状态
const QString OFFLINE("离线");
const QString CONNECTING("正在连接...");
const QString ONLINE("在线");
const QString SUBSCRIBING("正在接收行情...");
const QString DISCONNECTING("正在断开连接...");

//放置行情数据文件*.dat的前缀地址
const QString DAT_PREDIR("data/");

//全部合约的列表(合约代码)
std::map<QString, Instrument*> instruList;

//正在订阅的合约集合(合约代码)
std::set<QString> instruSet;

//交易日
QString tradeDate;

//数据库资料
QString DB_DRIVER_NAME;
QString DB_HOST_NAME;
QString DATABASE_NAME;
QString USER_NAME;
QString PASSWORD;