#pragma once

#include <qstringlist.h>

class DBWriterController {
public:
	DBWriterController() = default;
	void readyToWrite(QStringList filenames);
};