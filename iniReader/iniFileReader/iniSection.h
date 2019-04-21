#pragma once
#include <string.h>
#include "..\iIniFileReader\iIniSection.h"
namespace INI
{
class iniSection
	: public iIniSection
{
public:
	iniSection(const char * const name);
	virtual ~iniSection();
	virtual bool add(iIniItem * item);
	virtual const char * const getName();
	virtual iIniItem const * find(const char * const name);
	virtual void print() = 0;
private:
	char * mName;
};
}

