#include <stdio.h>
#include "iniItem.h"

INI::iniItem::iniItem(const std::string& line)
	: iIniItem(line)
{
	try
	{
		mKey.clear();
		mValue.clear();
		if (line.empty())
		{
			return;
		}

		size_t valueLen = line.find("=");
		if (valueLen == std::string::npos)
		{
			mKey = line;
			return;
		}
		size_t keyLen = valueLen;
		while (keyLen && line.at(keyLen - 1) == ' ')
		{
			--keyLen;
		}
		mKey = line.substr(0, keyLen);
		const size_t lineLen = line.length();
		if (valueLen == lineLen)
		{
			return;
		}
		valueLen++;
		while (valueLen < lineLen && line.at(valueLen) == ' ')
		{
			++valueLen;
		}
		mValue = line.substr(valueLen);
	}
	catch (std::exception & e)
	{
		std::string errMessage = "in iniItem::iniItem(line): ";
		errMessage += e.what();
		throw(std::exception(errMessage.c_str()));
	}
}

INI::iniItem::~iniItem()
{
}

bool INI::iniItem::operator==(const INI::iIniItem& other) const
{
	if (typeid(*this) != typeid(other))
	{
		return false;
	}
	return mKey == other.getKey()
		&& mValue == other.getValue();
}

bool INI::iniItem::operator<(const INI::iIniItem& other) const
{
	if (typeid(*this) != typeid(other))
	{
		return false;
	}
	return mKey < other.getKey()
		|| (mKey == other.getKey() && mValue < other.getValue());
}

void INI::iniItem::setKey(const std::string& to)
{
	mKey = to;
}

void INI::iniItem::setValue(const std::string& to)
{
	mValue = to;
}

const std::string& INI::iniItem::getKey() const
{
	return mKey;
}

const std::string& INI::iniItem::getValue() const
{
	return mValue;
}

void INI::iniItem::print() const
{
	printf("%s = %s\n", getKey().c_str(), getValue().c_str());
}
