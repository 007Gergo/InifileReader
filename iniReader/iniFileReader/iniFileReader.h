#pragma once
#include "..\iIniFileReader\iIniFileReader.h"

namespace INI
{
class iniFileReader :
	public iIniFileReader
{
public:
	iniFileReader(const char * const fileName);
	virtual ~iniFileReader();
	virtual bool load();
	virtual bool changed();
	virtual bool hasError();
	virtual void print();
private:
	bool isEmptyLine(const char * const line);
	bool isComment(const char * const line);
	bool isSection(const char * const line);
	bool isItem(const char * const line);

	bool setCurrentSection(const char * const line);

	bool mHasError;
	char * mFileName;
	char * mCurrentSection;

};
}

