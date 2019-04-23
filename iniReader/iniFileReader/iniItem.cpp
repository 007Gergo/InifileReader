#include <string.h>
#include <stdio.h>
#include "iniItem.h"

INI::iniItem::iniItem(const char * const line)
	: iIniItem(line)
{
	mKey = nullptr;
	mValue = nullptr;
	if (!line)
	{
		return;
	}
	const size_t lineLen = strlen(line);
	const char * itr = line;
	while (*itr && *itr != '=')
	{
		++itr;
	}
	if (!*itr)
	{
		setKey(line);
		setValue(nullptr);
		return;
	}
	size_t len = itr - line;
	//rigthtrim
	while (len && line[len - 1] == ' ')
	{
		--len;
	}
	//handling empty string as nullptr
	if (len)
	{
		mKey = new char[len + 1];
		strncpy_s(mKey, len + 1, line, len);
		mKey[len] = '\0';
	}
	setValue(++itr);
}

INI::iniItem::~iniItem()
{
	delete[] mKey;
	delete[] mValue;
}

void INI::iniItem::setKey(const char * const to)
{
	delete[] mKey;
	mKey = nullptr;

	//handling empty string as nullptr
	if (to && *to)
	{
		size_t len = strlen(to);
		// rigthtrim
		while (len && to[len - 1] == ' ')
		{
			--len;
		}
		if (len)
		{
			++len;
			mKey = new char[len];
			strcpy_s(mKey, len, to);
		}
	}
}

void INI::iniItem::setValue(const char * const to)
{
	delete[] mValue;
	mValue = nullptr;
	const char * itr = to;
	// leftrim
	while (itr && *itr == ' ')
	{
		++itr;
	}
	//handling empty string as nullptr
	if (itr && *itr)
	{
		const size_t len = strlen(itr) + 1;
		mValue = new char[len];
		strcpy_s(mValue, len, itr);
	}
}

bool INI::iniItem::hasKey()
{
	return mKey;
}

bool INI::iniItem::hasValue()
{
	return mValue;
}

const char * const INI::iniItem::getKey()
{
	return mKey;
}

const char * const INI::iniItem::getValue()
{
	return mValue;
}

void INI::iniItem::print()
{
	printf("%s = %s\n", hasKey() ? getKey() : "", hasValue() ? getValue() : "");
}
