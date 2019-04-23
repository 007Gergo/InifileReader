#include <stdio.h>
#include "chainedIniItem.h"

INI::chainedIniItem::chainedIniItem(iIniItem * iniItem)
	: iChainedIniItem(iniItem), mIniItem(iniItem), mNext(nullptr)
{
}

INI::chainedIniItem::~chainedIniItem()
{
}

void INI::chainedIniItem::setNext(INI::iChainedIniItem * to)
{
	mNext = to;
}

INI::iChainedIniItem * INI::chainedIniItem::next()
{
	return mNext;
}

INI::iIniItem * INI::chainedIniItem::get()
{
	return mIniItem;
}
