#pragma once
#include "..\iIniFileReader\iIniSection.h"
namespace INI
{
class chainedIniSection
{
public:
	chainedIniSection(iIniSection * iniSection);
	virtual ~chainedIniSection();
	void setNext(chainedIniSection * to);
	chainedIniSection * next();
	iIniSection * get();
private:
	iIniSection * mIniSection;
	chainedIniSection * mNext;
};
}