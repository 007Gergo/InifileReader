#pragma once
#include "..\iIniFileReader\iIniItem.h"
namespace INI
{
class chainedIniItem
{
public:
	chainedIniItem(iIniItem * iniItem);
	virtual ~chainedIniItem();
	void setNext(chainedIniItem * to);
	chainedIniItem * next();
	iIniItem * get();
private:
	iIniItem * mIniItem;
	chainedIniItem * mNext;
};
}

