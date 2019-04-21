#pragma once
#include "iIniItem.h"

namespace INI
{
class iIniSection
{
public:
	iIniSection(const char * const name) {};
	virtual ~iIniSection() {};
	virtual bool add(iIniItem * item) = 0;
	virtual const char * const getName() = 0;
	virtual iIniItem const * find(const char * const name) = 0;
	virtual void print() = 0;
};
}

