# 期货行情机
## 功能

**实时接收存储期货市场的行情**

## 使用说明
1. 配置ini/instrumentInfo.ini，输入期货信息(要订阅的期货先要在这里写好信息）。格式是“期货代码|期货名字”，如果需要大量订阅，可以从快期中导出期货代码和期货名字的名单然后复制进这个文件
2. 配置ini/onekeySub.ini，输入想要一键订阅的期货代码，格式是“期货代码a；期货代码b；...”
3. 修改（把表名改为相应的合约代码）并运行“数据库建表”文件夹内的sql脚本建，配置ini/db.ini文件，程序要把数据持久化到数据库时要用到里面的信息
4. 配置ini/front.ini文件，配置前置机地址。项目中默认的地址为模拟行情的地址

## 模拟环境截图
运行时
![image](https://raw.githubusercontent.com/zhuzhenpeng/CTP-MarketDataMachine/master/images/running.jpg)
<br>
<br>
数据存在data文件夹内
![image](https://raw.githubusercontent.com/zhuzhenpeng/CTP-MarketDataMachine/master/images/csv_example1.jpg)
<br>
<br>
csv里面存着行情数据
![image](https://raw.githubusercontent.com/zhuzhenpeng/CTP-MarketDataMachine/master/images/csv_example2.jpg)

## 环境
VS2013 + Qt5.3(32位) + mysql(32位)
