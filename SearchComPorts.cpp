#include "SearchComPorts.h"

using namespace std;

#define REG_ROOT HKEY_LOCAL_MACHINE
const wchar_t* REG_SUB=L"HARDWARE\\DEVICEMAP\\SERIALCOMM";

SearchComPorts::SearchComPorts()
{
}

SearchComPorts::~SearchComPorts()
{
}
	
bool SearchComPorts::listComPorts(wstring* portNameList,wstring* portList,int &nPorts)
{
	HKEY key;
	DWORD portNum=0;

	if(openRegister(REG_ROOT,REG_SUB,key))
	{		
		if(infoKey(key,portNum))
		{
			if(portNum<=MAX_PORT_NUM)
			{
				if(!enumValueKey(key,portNum,portNameList,portList)) regError="Error enumerating key";
			}
			else regError="Error too many ports";
		}
		else regError="Error getting info";
	}
	else regError="Error oppening register";

	RegCloseKey(key);

	if(regError=="")
	{
		nPorts=portNum;
		return true;
	}
	else return false;
}

bool SearchComPorts::openRegister(HKEY rootKey,const wchar_t* subKey,HKEY &regKey)
{
	//Open key for specified root and sub key, important to emitt the "KEY_WOW64_64KEY" on 64-bit systems
	LONG result=RegOpenKeyEx(rootKey,subKey,0,KEY_READ | KEY_QUERY_VALUE | KEY_WOW64_64KEY,&regKey);

	if(result!=ERROR_SUCCESS)
	{
		if(result==ERROR_FILE_NOT_FOUND) regSpError="FILE_NOT_FOUND"; //In this case this can mean that no com ports have been conencted since boot, so not necessarily fatal.
		else regSpError="Error opening key";

		return false;
	}
	else return true;
}

bool SearchComPorts::infoKey(HKEY regKey,DWORD &nValues)
{
	//Query info for how many current ports are connected, to know how many times to enum.
	LONG result=RegQueryInfoKey
	(
		regKey,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		&nValues,
		NULL,
		NULL,
		NULL,
		NULL
	);

	if(result!=ERROR_SUCCESS)
	{
		regSpError="Error querying info";
		return false;
	}
	else return true;
}

bool SearchComPorts::enumValueKey(HKEY regKey,DWORD nValues,wstring* nameArray,wstring* valueArray)
{
	//Var type string
	DWORD varType=REG_SZ;

	wchar_t nameBuff[MAX_REG_NAME_LEN]={0};
	wchar_t valueBuff[(MAX_REG_VALUE_SIZE/2)]={0};

	for(DWORD iCount=0;iCount<nValues;iCount++)
	{		
		DWORD maxNameLen=MAX_REG_NAME_LEN;
		DWORD maxValueSize=MAX_REG_VALUE_SIZE;		

		//Find value and name for corresponding iCount -> index count value
		LONG result=RegEnumValue
		(
			regKey,
			iCount,
			nameBuff,
			&maxNameLen,
			NULL,
			&varType,
			(LPBYTE)valueBuff,
			&maxValueSize
		);

		if(result!=ERROR_SUCCESS)
		{
			//Error occured enumerating the key
			if(result==ERROR_NO_MORE_ITEMS) regSpError="Error no more items";
			else if(result==ERROR_MORE_DATA) regSpError="Error more data";
			else regSpError="Error register enum";

			return false;
		}
		else 
		{
			//Otherwise put the value and name into the string arrays
			nameArray[iCount]=nameBuff;
			valueArray[iCount]=valueBuff;
		}

		memset(nameBuff,0,sizeof(nameBuff));
		memset(valueBuff,0,sizeof(valueBuff));
	}

	//If nothing went wrong return true
	return true;
}