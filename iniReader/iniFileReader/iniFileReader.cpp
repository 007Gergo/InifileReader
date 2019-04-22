#include <string.h>
#include <stdio.h>
#include "iniFileReader.h"
#include "iniSection.h"
#include "iniItem.h"

#define MAXBUFFERSIZE 1024

INI::iniFileReader::iniFileReader(const char * const fileName)
	: iIniFileReader(), mHasError(true), mFileName(nullptr), mIniSections(nullptr)
{
	const size_t len = strlen(fileName) + 1;
	mFileName = new char[len];
	strcpy_s(mFileName, len, fileName);
}

INI::iniFileReader::~iniFileReader()
{
	delete[] mFileName;
	clearIniSections();
}

void INI::iniFileReader::clearIniSections()
{
	chainedIniSection * current = mIniSections;
	while (current)
	{
		auto next = current->next();
		delete current->get();
		delete current;
		current = next;
	}
	mIniSections = nullptr;
	mLastIniSection = nullptr;
}

bool INI::iniFileReader::load()
{
	clearIniSections();
	mLastIniSection = nullptr;
	if (!mFileName)
	{
		printf("[ERR] Can't open file: file name is empty!\n");
		mHasError = true;
		return false;
	}

	FILE *file;
	if (fopen_s(&file, mFileName, "r") || !file)
	{
		printf("[ERR] Can't open file %s\n", mFileName);
		mHasError = true;
		return false;
	}

	char line[MAXBUFFERSIZE + 1];

	mHasError = false;
	int lineNo = 0;
	iIniSection * currentSection = nullptr;
	while (fgets(line, sizeof(line), file) != nullptr) /* read a line */
	{
		++lineNo;
		size_t lineLen = strlen(line);
		while (lineLen && (line[lineLen-1] == '\r' || line[lineLen-1] == '\n'))
		{
			line[lineLen-1] = '\0';
			--lineLen;
		}
		if (lineLen == MAXBUFFERSIZE)
		{
			printf("[ERR] line %d is to long: %s\n", lineNo, line);
			mHasError = true;
			continue;
		}
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
			bool isOK = false;
			const char * const currentName = getSectionName(line, isOK);
			if (!isOK)
			{
				printf("[ERR] Wrong foramated section in line %d: %s\n", lineNo, line);
				mHasError = true;
				continue;
			}
			currentSection = findSection(currentName);
			if (!currentSection)
			{
				currentSection = addSection(currentName);
			}
		}
		else if (isItem(line))
		{
			INI::iniItem * newItem = new INI::iniItem(line);
			if (!newItem->hasKey())
			{
				printf("[ERR] Wrong foramated key-value pair in line %d: %s\n", lineNo, line);
				mHasError = true;
			}
			if (!currentSection)
			{
				printf("[WRN] key-value pair not in a section in line %d: %s\n", lineNo, line);
				currentSection = addSection(nullptr);
			}
			currentSection->add(newItem);
		}
		else
		{
			printf("[ERR] Unknown line %d: %s\n", lineNo, line);
			mHasError = true;
		}
	}
	fclose(file);
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
	INI::chainedIniSection * iter = mIniSections;
	while (iter)
	{
		INI::iIniSection * iniSection = iter->get();
		if (iniSection)
		{
			iniSection->print();
		}
		iter = iter->next();
	}
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

const char * const INI::iniFileReader::getSectionName(const char * const line, bool & isOk)
{
	isOk = false;
	if (!line)
	{
		return nullptr;
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
		return nullptr;
	}

	const size_t len = itr - section;

	++itr;
	if (*itr)
	{
		/// extra character after ']'
		return nullptr;
	}

	char * currentSection = nullptr;
	if (len)
	{
		currentSection = new char[len + 1];
		strncpy_s(currentSection, len + 1, section, len);
		currentSection[len] = '\0';
	}
	isOk = true;
	return currentSection;
}

INI::iIniSection * INI::iniFileReader::findSection(const char * const name)
{
	chainedIniSection * current = mIniSections;
	while (current)
	{
		INI::iIniSection * section = current->get();
		if (!section)
		{
			continue;
		}
		const char * const currentName = section->getName();
		if (!name && !currentName)
		{
			return section;
		}
		if (!name || !currentName)
		{
			continue;
		}
		if (!strcmp(name, currentName))
		{
			return section;
		}
		current = current->next();
	}
	return nullptr;
}
INI::iIniSection * INI::iniFileReader::addSection(const char * const name)
{
	INI::iIniSection * aSection = new INI::iniSection(name);
	INI::chainedIniSection * newSection = new INI::chainedIniSection(aSection);
	if (mLastIniSection)
	{
		mLastIniSection->setNext(newSection);
	}
	else
	{
		mIniSections = newSection;
	}
	mLastIniSection = newSection;
	return aSection;
}
