#include <string.h>
#include <stdio.h>
#include "iniFileReader.h"

INI::iniFileReader::iniFileReader(const char * const fileName) 
	: iIniFileReader(), mHasError(true), mFileName(nullptr), mCurrentSection(nullptr)
{
	const size_t len = strlen(fileName) + 1;
	mFileName = new char[len];
	strcpy_s(mFileName, len, fileName);
}

INI::iniFileReader::~iniFileReader()
{
	delete[] mFileName;
	delete[] mCurrentSection;
}

bool INI::iniFileReader::load()
{	
	mHasError = false;
	int lineNo = 0;
	while (false)
	{
		++lineNo;
		const char * line = " ";

		if (isEmptyLine(line))
		{
			continue;
		}
		if (isComment(line))
		{
			continue;
		}

		if (isSection(line))
		{
			if(!setCurrentSection(line))
			{
				printf("[ERR] Wrong foramated section in line %d: %s", lineNo, line);
				mHasError = true;
				continue;
			}
		}
		else if (isItem(line))
		{
		}
		else
		{
			printf("[ERR] Unknown line %d: %s", lineNo, line);
			mHasError = true;
		}
	}
	return mHasError;
}

bool INI::iniFileReader::changed()
{
	return true;
}

bool INI::iniFileReader::hasError()
{
	return mHasError;
}

void INI::iniFileReader::print()
{ 
	printf("Print..\n");
}

bool INI::iniFileReader::isEmptyLine(const char * const line)
{
	if (!line)
	{
		return true;
	}
	const char * itr = line;
	while (*itr == ' ' || *itr == '\t' )
	{
		++itr;
	}
	return  !*itr;
}

bool INI::iniFileReader::isComment(const char * const line)
{
	if (!line)
	{
		return false;
	}
	return line == ";";
}

bool INI::iniFileReader::isSection(const char * const line)
{
	if (!line)
	{
		return false;
	}
	return *line == '[';
}

bool INI::iniFileReader::isItem(const char * const line)
{
	if (!line)
	{
		return false;
	}
	const char * itr = line;
	while (*itr && *itr != '=')
	{
		++itr;
	}
	return  *itr == '=';
}

bool INI::iniFileReader::setCurrentSection(const char * const line)
{
	if (!line)
	{
		return false;
	}
	const char * section = line;
	++section;
	const char * itr = section;
	while (*itr && *itr != ']')
	{
		++itr;
	}

	if (!*itr)
	{
		/// missing ']'
		return false;
	}

	const size_t len = itr - section;

	++itr;
	if (*itr)
	{
		/// extra character after ']'
		return false;
	}

	delete mCurrentSection;
	mCurrentSection = new char[len + 1];
	if (len)
	{
		strncpy_s(mCurrentSection, len + 1, section, len);
	}
	mCurrentSection[len] = '\0';
	return true;
}