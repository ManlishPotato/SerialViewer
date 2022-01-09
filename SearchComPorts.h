#pragma once

#include <iostream>
#include <windows.h>

#define MAX_PORT_NUM 10 //Max number of used ports at a given time
#define MAX_REG_VALUE_SIZE 64 //Max register value size in bytes
#define MAX_REG_NAME_LEN 32 //Max register name length in 16-bits

class SearchComPorts
{	
	public:
		SearchComPorts();
		~SearchComPorts();
	
		bool listComPorts(std::wstring* portNameList,std::wstring *portList,int &nPorts);

	private:		
		bool openRegister(HKEY rootKey,const wchar_t* subKey,HKEY &regKey);
		bool infoKey(HKEY regKey,DWORD &nValues);
		bool enumValueKey(HKEY regKey,DWORD nValues,std::wstring* nameArray,std::wstring* valueArray);

	public:
		
		std::string regError=""; //Top level error message
		std::string regSpError=""; //More detailed error message
};

