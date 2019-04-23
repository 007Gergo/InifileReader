#include "iniSection.h"

INI::iniSection::iniSection(const char * const name)
	: iIniSection(name), mName(nullptr), mIniItems(nullptr), mLastIniItem(nullptr)
{
	// handling empty string as nullptr
	if (name && *name)
	{
		const size_t len = strlen(name) + 1;
		mName = new char[len];
		strcpy_s(mName, len, name);
	}
}

INI::iniSection::~iniSection()
{
	delete[] mName;
	iChainedIniItem * current = mIniItems;
	while (current)
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

INI::iChainedIniItem * INI::iniSection::getIninItems()
{
	return mIniItems;
}

INI::iIniItem const * INI::iniSection::find(const char * const key)
{
	if (key && !key)
	{
		// convert empty string to nullptr;
		find(nullptr);
	}
	INI::iChainedIniItem * current = mIniItems;
	while (current)
	{
		INI::iIniItem * iniItem = current->get();
		if (!iniItem)
		{
			continue;
		}
		const char * const currentKey = iniItem->getKey();
		if (!key && !currentKey)
		{
			return iniItem;
		}
		if (!key || !currentKey)
		{
			continue;
		}
		if (!strcmp(key, currentKey))
		{
			return iniItem;
		}
		current = current->next();
	}
	return nullptr;
}

void INI::iniSection::print()
{
	if (mName)
	{
		printf("[%s]\n", mName);
	}
	INI::iChainedIniItem * iter = mIniItems;
	while (iter)
	{
		INI::iIniItem *	iniItem = iter->get();
		if (iniItem)
		{
			iniItem->print();
		}
		iter = iter->next();
	}
	printf("\n");
}
