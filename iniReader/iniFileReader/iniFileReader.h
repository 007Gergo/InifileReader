#pragma once
#include "..\iIniFileReader\iIniFileReader.h"
#include "..\iIniFileReader\iChainedIniSection.h"
#include "..\iIniFileReader\iChainedIniItem.h"

namespace INI
{
	class iniFileReader :
		public iIniFileReader
	{
	public:
		iniFileReader(const char * const fileName);
		virtual ~iniFileReader();
		virtual iChainedIniSection * getSections();
		virtual bool load();
		virtual bool reloadIfChanged();
		virtual bool hasError();
		virtual void print();
		virtual iIniSection * findSection(const char * const name);

	private:
		bool isEmptyLine(const char * const line);
		bool isComment(const char * const line);
		bool isSection(const char * const line);
		bool isItem(const char * const line);

		void clearIniSections(iChainedIniSection * iniSections);
		const char * const getSectionName(const char * const line, bool & isOk);
		iIniSection * addSection(const char * const name);
		bool getFileTimes(FILETIME * ftCreate, FILETIME *ftAccess, FILETIME *ftWrite);
		bool isSameSection(iChainedIniSection * oldSection, iChainedIniSection * newSection);
		bool isSameItem(iChainedIniItem * oldItem, iChainedIniItem * newItem);

		bool mHasError;
		char * mFileName;
		iChainedIniSection * mIniSections;
		iChainedIniSection * mLastIniSection;
		FILETIME mFtCreat;
		FILETIME mFtAccess;
		FILETIME mFtWrite;
	};
}
