#include "chainedIniSection.h"
#include <stdio.h>
#include "chainedIniItem.h"

INI::chainedIniSection::chainedIniSection(iIniSection * iniSection)
	: mIniSection(iniSection), mNext(nullptr)
{
}

INI::chainedIniSection::~chainedIniSection()
{
}

void INI::chainedIniSection::setNext(INI::chainedIniSection * to)
{
	mNext = to;
}

INI::chainedIniSection * INI::chainedIniSection::next()
{
	return mNext;
}

INI::iIniSection * INI::chainedIniSection::get()
{
	return mIniSection;
}
