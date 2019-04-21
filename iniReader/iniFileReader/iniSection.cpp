#include "iniSection.h"

INI::iniSection::iniSection(const char * const name)
	: iIniSection(name), mName(nullptr), mIniItems(nullptr), mLastIniItem(nullptr)
{
	if (name)
	{
		const size_t len = strlen(name) + 1;
		mName = new char[len];
		strcpy_s(mName, len, name);
	}
}

INI::iniSection::~iniSection()
{
	delete[] mName;
	chainedIniItem * current = mIniItems;
	while(current)
	{	
		auto next = current->next();
		delete current->get();
		delete current;
		current = next;
	}
}

bool INI::iniSection::add(iIniItem * item)
{
	INI::chainedIniItem * newItem = new INI::chainedIniItem(item);
	if (mLastIniItem)
	{
		mLastIniItem->setNext(newItem);
	}
	else
	{
		mIniItems = newItem;
	}
	mLastIniItem = newItem;
	return true;
}

const char * const INI::iniSection::getName()
{
	return mName;
}

INI::iIniItem const * INI::iniSection::find(const char * const name)
{
	return nullptr;
}

void INI::iniSection::print()
{
	printf("........\n");
	if (mName)
	{
		printf("[%s]\n", mName);
	}
	printf("........\n");
	INI::chainedIniItem * iter = mIniItems;
	while (iter)
	{
		INI::iIniItem *	iniItem = iter->get();
		iniItem->print();
		iter = iter->next();
	}
	printf("-------\n");
}
