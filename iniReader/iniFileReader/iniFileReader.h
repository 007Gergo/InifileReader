#pragma once
#include "..\iIniFileReader\iIniFileReader.h"

namespace INI
{
class iniFileReader :
	public iIniFileReader
{
public:
	iniFileReader(const char * fileName);
	virtual ~iniFileReader();
	virtual bool load();
	virtual bool changed();
	virtual void print();
};
}

