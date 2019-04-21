#include <stdio.h>
#include "iniFileReader.h"

namespace INI
{	
iniFileReader::iniFileReader(const char * fileName) : iIniFileReader()
{
}

iniFileReader::~iniFileReader()
{
}

bool iniFileReader::load()
{ 
	return true;
}

bool iniFileReader::changed()
{
	return true;
}

void iniFileReader::print()
{ 
	printf("Print..\n");
}
}
