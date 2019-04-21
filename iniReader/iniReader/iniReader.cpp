#include "pch.h"
#include <stdio.h>
#include <windows.h>

#include "..\iniFileReader\iniFileReader.h"
#include "..\iniFileReader\iniItem.h"

int main(int argv, char** argc)
{
	const char * fileName = "file.ini";
	INI::iIniFileReader * ini = new INI::iniFileReader(fileName);
	ini->load();
	ini->print();
	while (!ini->changed())
	{
		Sleep(5000);
	}
	ini->load();
	ini->print();

	{
		// testing iniitem TODO: remove 
		INI::iniItem i1(nullptr);
		i1.print();
		INI::iniItem i2("");
		i2.print();
		INI::iniItem i3("=");
		i3.print();
		INI::iniItem i4("key");
		i4.print();
		INI::iniItem i5("key=");
		i5.print();
		INI::iniItem i6("key=value");
		i6.print();
		INI::iniItem i7("=value");
		i7.print();
	}

	delete ini;
}
