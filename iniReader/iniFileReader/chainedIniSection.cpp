#include <stdio.h>
#include "chainedIniSection.h"

INI::chainedIniSection::chainedIniSection(iIniSection * iniSection)
	: iChainedIniSection(iniSection), mIniSection(iniSection), mNext(nullptr)
{
}

INI::chainedIniSection::~chainedIniSection()
{
}

void INI::chainedIniSection::setNext(INI::iChainedIniSection * to)
{
	mNext = to;
}

INI::iChainedIniSection * INI::chainedIniSection::next()
{
	return mNext;
}

INI::iIniSection * INI::chainedIniSection::get()
{
	return mIniSection;
}
