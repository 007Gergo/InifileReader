#include <string.h>
#include <stdio.h>
#include <windows.h>
#include "iniFileReader.h"
#include "chainedIniSection.h"
#include "chainedIniItem.h"
#include "iniSection.h"
#include "iniItem.h"

#define MAXBUFFERSIZE 1024

bool isSame(const char * const str1, const char * const str2)
{
	if (!str1 && !str2)
	{
		return true;
	}
	if (!str1 && !str2)
	{
		return true;
	}
	if (!str1 || !str2)
	{
		return false;
	}
	return !strcmp(str1, str2);
};


INI::iniFileReader::iniFileReader(const char * const fileName)
	: iIniFileReader(), mHasError(true), mFileName(nullptr), mIniSections(nullptr)
{
	if (fileName && fileName)
	{
		const size_t len = strlen(fileName) + 1;
		mFileName = new char[len];
		strcpy_s(mFileName, len, fileName);
	}
	mFtCreat = { 0, 0 };
	mFtAccess = { 0, 0 };
	mFtWrite = { 0, 0 };
}

INI::iniFileReader::~iniFileReader()
{
	delete[] mFileName;
	clearIniSections(mIniSections);
}

INI::iChainedIniSection * INI::iniFileReader::getSections()
{
	return mIniSections;
};

void INI::iniFileReader::clearIniSections(iChainedIniSection * iniSections)
{
	iChainedIniSection * current = iniSections;
	while (current)
	{
		auto next = current->next();
		delete current->get();
		delete current;
		current = next;
	}
	iniSections = nullptr;
}

bool INI::iniFileReader::load()
{
	clearIniSections(mIniSections);
	mLastIniSection = nullptr;
	if (!mFileName)
	{
		printf("[ERR] Can't open file: file name is empty!\n");
		mHasError = true;
		return false;
	}
	if (!getFileTimes(&mFtCreat, &mFtAccess, &mFtWrite))
	{
		printf("[ERR] Can't open file %s\n", mFileName);
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
		while (lineLen && (line[lineLen - 1] == '\r' || line[lineLen - 1] == '\n'))
		{
			line[lineLen - 1] = '\0';
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
			INI::iIniItem * newItem = new INI::iniItem(line);
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

bool INI::iniFileReader::reloadIfChanged()
{
	FILETIME ftCreate, ftAccess, ftWrite;
	if (!getFileTimes(&ftCreate, &ftAccess, &ftWrite))
	{
		return true;
	}
	bool same = ftCreate.dwHighDateTime == mFtCreat.dwHighDateTime
		&& ftCreate.dwLowDateTime == mFtCreat.dwLowDateTime
		&& ftWrite.dwHighDateTime == mFtWrite.dwHighDateTime
		&& ftWrite.dwLowDateTime == mFtWrite.dwLowDateTime;
	if (same)
	{
		return false;
	}
	// file changed on disk ...
	iChainedIniSection * oldIniSections = mIniSections;
	mIniSections = nullptr;
	load();
	iChainedIniSection * oldSection = oldIniSections;
	iChainedIniSection * newSection = mIniSections;
	while (oldSection && newSection)
	{
		if (!isSameSection(newSection, oldSection))
		{
			break;
		}
		oldSection = oldSection->next();
		newSection = newSection->next();
	}
	same = !oldSection && !newSection;
	return !same;
}

bool INI::iniFileReader::isSameSection(iChainedIniSection * oldSection, iChainedIniSection * newSection)
{
	const auto oldIniSection = oldSection->get();
	const auto newIniSection = newSection->get();
	if (!oldIniSection && !newIniSection)
	{
		return true;
	}
	if (!oldIniSection || !newIniSection)
	{
		return false;
	}

	const char * const oldSectionName = oldIniSection->getName();
	const char * const newSectionName = newIniSection->getName();
	if (!isSame(oldSectionName, newSectionName))
	{
		return false;
	}

	iChainedIniItem * oldItem = oldIniSection->getIninItems();
	iChainedIniItem * newItem = newIniSection->getIninItems();
	while (oldItem && newItem)
	{
		if (!isSameItem(oldItem, newItem))
		{
			break;
		}
		oldItem = oldItem->next();
		newItem = newItem->next();
	}
	return (!oldItem && !newItem);
}

bool INI::iniFileReader::isSameItem(iChainedIniItem * oldItem, iChainedIniItem * newItem)
{
	const auto oldIniItem = oldItem->get();
	const auto newIniItem = newItem->get();
	if (!oldIniItem && !newIniItem)
	{
		return true;
	}
	if (!oldIniItem || !newIniItem)
	{
		return false;
	}
	return isSame(oldIniItem->getKey(), newIniItem->getKey())
		&& isSame(oldIniItem->getValue(), newIniItem->getValue());
}

bool INI::iniFileReader::getFileTimes(FILETIME * ftCreate, FILETIME *ftAccess, FILETIME *ftWrite)
{
	if (!mFileName || !ftCreate || !ftAccess || !ftWrite)
	{
		return false;
	}
	HANDLE hFile = CreateFile(mFileName, GENERIC_READ, FILE_SHARE_READ, NULL,
		OPEN_EXISTING, 0, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		return false;
	}
	if (!GetFileTime(hFile, ftCreate, ftAccess, ftWrite))
	{
		return false;
	}
	CloseHandle(hFile);
	return true;
}

bool INI::iniFileReader::hasError()
{
	return mHasError;
}

void INI::iniFileReader::print()
{
	printf("file '%s':\n", mFileName ? mFileName : "");
	INI::iChainedIniSection * iter = mIniSections;
	while (iter)
	{
		INI::iIniSection * iniSection = iter->get();
		if (iniSection)
		{
			iniSection->print();
		}
		iter = iter->next();
	}
	printf("\n");
}

bool INI::iniFileReader::isEmptyLine(const char * const line)
{
	if (!line)
	{
		return true;
	}
	const char * itr = line;
	while (*itr == ' ' || *itr == '\t')
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
	return *line == ';';
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
	INI::iChainedIniSection * current = mIniSections;
	while (current)
	{
		INI::iIniSection * section = current->get();
		if (!section)
		{
			continue;
		}
		const char * const currentName = section->getName();
		if (isSame(name, currentName))
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
	INI::iChainedIniSection * newSection = new INI::chainedIniSection(aSection);
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
