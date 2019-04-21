#include "iniSection.h"

INI::iniSection::iniSection(const char * const name)
	: iIniSection(name)
{
	mName = nullptr;

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
}

bool INI::iniSection::add(iIniItem * item)
{
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
}
