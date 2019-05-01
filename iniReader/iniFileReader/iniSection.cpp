#include <stdio.h>
#include "iniSection.h"

INI::iniSection::iniSection(const std::string& name)
	: iIniSection(name), mIniItems({})
{
	mName = name;
}

INI::iniSection::~iniSection()
{
	for (auto &current : mIniItems)
	{
		delete current;
	}
}

bool INI::iniSection::operator==(const INI::iIniSection& other) const
{
	if (typeid(*this) != typeid(other))
	{
		return false;
	}
	return mName == other.getName();
}

bool INI::iniSection::operator<(const INI::iIniSection& other) const
{
	if (typeid(*this) != typeid(other))
	{
		return false;
	}
	return mName < other.getName();
}

template <typename T> bool pComp(const T * const & a, const T * const & b)
{
	if (!a || !b)
	{
		return false;
	}
	if (!a)
	{
		return true;
	}
	if (!b)
	{
		return false;
	}
	return *a < *b;
}

bool INI::iniSection::add(iIniItem * item)
{
	try
	{
		mIniItems.emplace_back(item);
		mIniItems.sort(pComp<INI::iIniItem>);
	}
	catch (std::exception & e)
	{
		std::string errMessage = "in iniSectiom::add(): ";
		errMessage += e.what();
		throw(std::exception(errMessage.c_str()));
	}
	return true;
}

const std::string & INI::iniSection::getName() const
{
	return mName;
}

const std::list<INI::iIniItem *> & INI::iniSection::getIniItems() const
{
	return mIniItems;
}

INI::iIniItem const * INI::iniSection::find(const std::string & key) const
{
	try
	{
		for (const auto & iniItem : mIniItems)
		{
			if (iniItem && iniItem->getKey() == key)
			{
				return iniItem;
			}
		}
	}
	catch (std::exception & e)
	{
		std::string errMessage = "in iniSection::find(): ";
		errMessage += e.what();
		throw(std::exception(errMessage.c_str()));
	}
	return nullptr;
}

void INI::iniSection::print() const
{
	try
	{
		if (!mName.empty())
		{
			printf("[%s]\n", mName.c_str());
		}
		for (auto& iniItem : mIniItems)
		{
			if (iniItem)
			{
				iniItem->print();
			}
		}
		printf("\n");
	}
	catch (std::exception & e)
	{
		std::string errMessage = "in iniSection::print(): ";
		errMessage += e.what();
		throw(std::exception(errMessage.c_str()));
	}
}
