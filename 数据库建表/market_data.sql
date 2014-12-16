SET FOREIGN_KEY_CHECKS=0;

-- ----------------------------
-- Table structure for a1501
-- ----------------------------
DROP TABLE IF EXISTS `a1501`;
CREATE TABLE `a1501` (
  `d` date NOT NULL DEFAULT '0000-00-00',
  `t` time NOT NULL DEFAULT '00:00:00',
  `ms` int(11) NOT NULL DEFAULT '0',
  `LastPrice` double DEFAULT NULL,
  `HighestPrice` double DEFAULT NULL,
  `LowestPrice` double DEFAULT NULL,
  `BidPrice1` double DEFAULT NULL,
  `BidVolume1` int(11) DEFAULT NULL,
  `AskPrice1` double DEFAULT NULL,
  `AskVolume1` int(11) DEFAULT NULL,
  `Volume` int(11) DEFAULT NULL,
  `OpenInterest` int(11) DEFAULT NULL,
  `Turnover` double DEFAULT NULL,
  `ClosePrice` double DEFAULT NULL,
  `AveragePrice` double DEFAULT NULL,
  PRIMARY KEY (`d`,`t`,`ms`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- ----------------------------
-- Table structure for a1501_day
-- ----------------------------
DROP TABLE IF EXISTS `a1501_day`;
CREATE TABLE `a1501_day` (
  `d` date NOT NULL DEFAULT '0000-00-00',
  `OpenPrice` double DEFAULT NULL,
  `UpperLimitPrice` double DEFAULT NULL,
  `LowerLimitPrice` double DEFAULT NULL,
  `PreClosePrice` double DEFAULT NULL,
  `PreOpenInterest` double DEFAULT NULL,
  `PreSettlementPrice` double DEFAULT NULL,
  PRIMARY KEY (`d`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
