#pragma once
#include "..\iIniFileReader\iIniFileReader.h"
#include "chainedIniSection.h"

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

	void clearIniSections();
	const char * const getSectionName(const char * const line, bool & isOk);
	iIniSection * findSection(const char * const name);
	iIniSection * addSection(const char * const name);
	bool mHasError;
	char * mFileName;
	chainedIniSection * mIniSections;
	chainedIniSection * mLastIniSection;
};
}

