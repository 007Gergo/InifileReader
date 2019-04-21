#include <stdio.h>
#include "chainedIniItem.h"

INI::chainedIniItem::chainedIniItem(iIniItem * iniItem) 
	: mIniItem(iniItem), mNext(nullptr)
{
}

INI::chainedIniItem::~chainedIniItem()
{
}

void INI::chainedIniItem::setNext(INI::chainedIniItem * to)
{
	mNext = to;
}

INI::chainedIniItem * INI::chainedIniItem::next()
{
	return mNext;
}

INI::iIniItem * INI::chainedIniItem::get()
{
	return mIniItem;
}
