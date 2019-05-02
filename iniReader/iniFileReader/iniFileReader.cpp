#include <stdio.h>
#include <io.h>
#include "iniFileReader.h"
#include "iniSection.h"
#include "iniItem.h"

#define MAXBUFFERSIZE 1024

INI::iniFileReader::iniFileReader(const std::string &name)
	: iIniFileReader(name)
	, mName(name), mHasError(true)
	, mFtCreat(0), mFtAccess(0), mFtWrite(0)
{
}

INI::iniFileReader::~iniFileReader()
{
	clearIniSections(mIniSections);
}

const std::list<INI::iIniSection *> & INI::iniFileReader::getSections() const
{
	return mIniSections;
};

void INI::iniFileReader::clearIniSections(std::list<INI::iIniSection *> & iniSections)
{
	try
	{
		for (auto &current : iniSections)
		{
			delete current;
		}
	}
	catch (std::exception & e)
	{
		std::string errMessage = "in iniFileReader::clearIniSections(): ";
		errMessage += e.what();
		throw(std::exception(errMessage.c_str()));
	}
}

bool INI::iniFileReader::load()
{
	try {
		mHasError = true;
		clearIniSections(mIniSections);
		if (mName.empty())
		{
			std::string errMessage = "in iniFileReader::load(): ";
			errMessage += "Can't open file: file name is empty!\n";
			throw(std::exception(errMessage.c_str()));
			return false;
		}
		if (!getFileTimes(mFtCreat, mFtAccess, mFtWrite))
		{
			std::string errMessage = "in iniFileReader::load(): ";
			errMessage += "Can't open file: " + mName + "\n";
			throw(std::exception(errMessage.c_str()));
			return false;
		}
		FILE *file;
		if (fopen_s(&file, mName.c_str(), "r") || !file)
		{
			std::string errMessage = "in iniFileReader::load(): ";
			errMessage += "Can't open file: " + mName + "\n";
			throw(std::exception(errMessage.c_str()));
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
				const std::string & currentName = getSectionName(line, isOK);
				if (!isOK)
				{
					printf("[ERR] Wrong foramated section in line %d: %s\n", lineNo, line);
					mHasError = true;
					continue;
				}
				currentSection = addSection(currentName);
			}
			else if (isItem(line))
			{
				INI::iIniItem * newItem = new INI::iniItem(line);
				if (!newItem)
				{
					throw(std::exception("Out of memory.\n"));
				}
				if (newItem->getKey().empty())
				{
					printf("[ERR] Wrong foramated key-value pair in line %d: %s\n", lineNo, line);
					mHasError = true;
				}
				if (!currentSection)
				{
					printf("[WRN] key-value pair not in a section in line %d: %s\n", lineNo, line);
					currentSection = addSection("");
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
		if (mHasError)
		{
			throw(std::exception("Wrong foramated file. See the errors above.\n"));
		}
	}
	catch (std::exception & e)
	{
		mHasError = true;
		std::string errMessage = "in iniFileReader::load(): ";
		errMessage += e.what();
		throw(std::exception(errMessage.c_str()));
	}
	return mHasError;
}

bool INI::iniFileReader::reloadIfChanged()
{
	try
	{
		time_t ftCreate, ftAccess, ftWrite;
		if (!getFileTimes(ftCreate, ftAccess, ftWrite))
		{
			return true;
		}
		bool same = ftCreate == mFtCreat
			&& ftWrite == mFtWrite;
		if (same)
		{
			return false;
		}
		// file changed on disk ...
		std::list<iIniSection *> oldIniSections = mIniSections;
		mIniSections.clear();
		load();
		auto oldSectionItr = oldIniSections.cbegin();
		auto newSectionItr = mIniSections.cbegin();
		while (oldSectionItr != oldIniSections.cend()
			&& newSectionItr != mIniSections.cend())
		{
			if (!isSameSection(*oldSectionItr, *newSectionItr))
			{
				return true;
			}
			++oldSectionItr;
			++newSectionItr;
		}
		return !(oldSectionItr == oldIniSections.cend()
			&& newSectionItr == mIniSections.cend());
	}
	catch (std::exception & e)
	{
		std::string errMessage = "in iniFileReader::reloadIfChanged(): ";
		errMessage += e.what();
		throw(std::exception(errMessage.c_str()));
	}
}

bool INI::iniFileReader::isSameSection(const iIniSection * const oldIniSection, const iIniSection * const newIniSection) const
{
	try
	{
		if (!oldIniSection && !newIniSection)
		{
			return true;
		}
		if (!oldIniSection || !newIniSection)
		{
			return false;
		}

		if (!(*oldIniSection == *newIniSection))
		{
			return false;
		}

		const auto & oldIniItems = oldIniSection->getIniItems();
		const auto & newIniItems = newIniSection->getIniItems();

		auto oldItemItr = oldIniItems.cbegin();
		auto newItemItr = newIniItems.cbegin();
		while (oldItemItr != oldIniItems.cend()
			&& newItemItr != newIniItems.cend())
		{
			const iIniItem * oldItem = *oldItemItr;
			const iIniItem * newItem = *newItemItr;

			if (!oldItem || !newItem)
			{
				return false;
			}
			else if (oldItem && newItem)
			{
				if (!(*oldItem == *newItem))
				{
					return false;
				}
			}
			++oldItemItr;
			++newItemItr;
		}
		return (oldItemItr == oldIniItems.cend()
			&& newItemItr == newIniItems.cend());
	}
	catch (std::exception & e)
	{
		std::string errMessage = "in iniFileReader::isSameSection(): ";
		errMessage += e.what();
		throw(std::exception(errMessage.c_str()));
	}
}

bool INI::iniFileReader::getFileTimes(time_t & ftCreate, time_t & ftAccess, time_t & ftWrite)
{
	try {
		if (mName.empty())
		{
			return false;
		}
		struct _finddata_t c_file;

		intptr_t hFile = _findfirst(mName.c_str(), &c_file);
		if (hFile == -1)
		{
			return false;
		}
		ftCreate = c_file.time_create;
		ftAccess = c_file.time_access;
		ftWrite = c_file.time_write;
		if (_findnext(hFile, &c_file) == -1 && errno != ENOENT)
		{
			return false;
		}
		_findclose(hFile);
	}
	catch (std::exception & e)
	{
		std::string errMessage = "in iniFileReader::getFileTimes(): ";
		errMessage += e.what();
		throw(std::exception(errMessage.c_str()));
	}
	return true;
}

bool INI::iniFileReader::hasError() const
{
	return mHasError;
}

void INI::iniFileReader::print() const
{
	try
	{
		printf("file '%s':\n", mName.c_str());
		for (const auto & iniSection : mIniSections)
		{
			if (iniSection)
			{
				iniSection->print();
			}
		}
		printf("\n");
	}
	catch (std::exception & e)
	{
		std::string errMessage = "in iniFileReader::print(): ";
		errMessage += e.what();
		throw(std::exception(errMessage.c_str()));
	}

}

bool INI::iniFileReader::isEmptyLine(const std::string & line)
{
	try {
		return line.find_first_not_of(" \t") == std::string::npos;
	}
	catch (std::exception & e)
	{
		std::string errMessage = "in iniFileReader::isEmptyLine(): ";
		errMessage += e.what();
		throw(std::exception(errMessage.c_str()));
	}
	return false;
}

bool INI::iniFileReader::isComment(const std::string & line)
{
	try {
		if (line.empty())
		{
			return false;
		}
		return line.at(0) == ';';
	}
	catch (std::exception & e)
	{
		std::string errMessage = "in iniFileReader::isComment(): ";
		errMessage += e.what();
		throw(std::exception(errMessage.c_str()));
	}
	return false;
}

bool INI::iniFileReader::isSection(const std::string & line)
{
	try {
		if (line.length() < 2)
		{
			return false;
		}
		return line.at(0) == '[' && line.at(line.length() - 1) == ']';
	}
	catch (std::exception & e)
	{
		std::string errMessage = "in iniFileReader::isSection(): ";
		errMessage += e.what();
		throw(std::exception(errMessage.c_str()));
	}
	return false;
}

bool INI::iniFileReader::isItem(const std::string & line)
{
	try
	{
		return line.find("=") != std::string::npos;
	}
	catch (std::exception & e)
	{
		std::string errMessage = "in iniFileReader::isItem(): ";
		errMessage += e.what();
		throw(std::exception(errMessage.c_str()));
	}
}

const std::string INI::iniFileReader::getSectionName(const std::string& line, bool & isOk)
{
	isOk = false;
	try {
		if (!isSection(line))
		{
			return "";
		}
		isOk = true;
		return line.substr(1, line.length() - 2);
	}
	catch (std::exception & e)
	{
		std::string errMessage = "in iniFileReader::getSectionName(): ";
		errMessage += e.what();
		throw(std::exception(errMessage.c_str()));
	}
	return "";
}

INI::iIniSection * INI::iniFileReader::findSection(const std::string & name) const
{
	try
	{
		for (const auto & iniSection : mIniSections)
		{
			if (iniSection && iniSection->getName() == name)
			{
				return iniSection;
			}
		}
	}
	catch (std::exception & e)
	{
		std::string errMessage = "in iniFileReader::findSection(): ";
		errMessage += e.what();
		throw(std::exception(errMessage.c_str()));
	}
	return nullptr;
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

INI::iIniSection * INI::iniFileReader::addSection(const std::string & name)
{
	try
	{
		INI::iIniSection * aSection = findSection(name);
		if (aSection)
		{
			return aSection;
		}
		aSection = new INI::iniSection(name);
		if (!aSection)
		{
			throw(std::exception("Out of memory.\n"));
		}
		mIniSections.emplace_back(aSection);
		mIniSections.sort(pComp<INI::iIniSection>);
		return aSection;
	}
	catch (std::exception & e)
	{
		std::string errMessage = "in iniFileReader::addSection(): ";
		errMessage += e.what();
		throw(std::exception(errMessage.c_str()));
	}
	return nullptr;
}
