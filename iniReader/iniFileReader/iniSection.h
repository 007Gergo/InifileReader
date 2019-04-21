#pragma once
#include <string.h>
#include <stdio.h>
#include "..\iIniFileReader\iIniSection.h"
#include "chainedIniItem.h"
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
	virtual void print();
private:
	char * mName;
	chainedIniItem * mIniItems;
	chainedIniItem * mLastIniItem;
};
}

