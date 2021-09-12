#include "serialClass.h"
using namespace std;

//Buffer size standards
#define rbs 256
#define tbs 256

//Read buffer
char readBuffer[rbs]={0};
int rbrp=0;
int rbwp=0;

//Write buffer
char writeBuffer[tbs]={0};
int wbrp=0;
int wbwp=0;

HANDLE portHandle; //Handle for the comm port

BOOL WINAPI CtrlHandler(DWORD fdwCtrlType);
void errorHandler(string msg);

serialClass::serialThreads::serialThreads()
{
	SetConsoleCtrlHandler(CtrlHandler,true);
}

void serialClass::serialThreads::startThreads()
{
	readThreadState=true;
	writeThreadState=true;
	readThread=thread(&serialClass::serialThreads::readThreadFn,this);
	writeThread=thread(&serialClass::serialThreads::writeThreadFn,this);	
}

void serialClass::serialThreads::endThreads()
{
	if(readThreadState | writeThreadState)
	{
		readThreadState=false;
		writeThreadState=false;
		readThread.join();
		writeThread.join();
	}
}

bool serialClass::serialThreads::readThreadFn()
{		
	OVERLAPPED readOv={0};
	readOv.hEvent=CreateEvent(NULL,true,false,NULL);
	if(readOv.hEvent==NULL) {CloseHandle(readOv.hEvent); errorHandler("Error creating read event handle"); return false;}

	DWORD evMask=0;

	while(readThreadState)
	{
		WaitCommEvent(portHandle,&evMask,&readOv);
		if(WaitForSingleObject(readOv.hEvent,INFINITE)==WAIT_OBJECT_0)
		{
			DWORD bytesRead=0;

			do
			{
				bytesRead=0;
				char dataInBuff[100]={0};

				bool state=false;
				unique_lock<mutex> lck(classMutex);					
				if(!ReadFile(portHandle,dataInBuff,sizeof(dataInBuff),NULL,&readOv))
				{
					DWORD lastError=GetLastError();
					lck.unlock();
					if(lastError==ERROR_IO_PENDING)
					{							
						if(WaitForSingleObject(readOv.hEvent,INFINITE)==WAIT_OBJECT_0)
						{
							if(GetOverlappedResult(portHandle,&readOv,&bytesRead,false))
							{
								bufferReadData(dataInBuff,bytesRead);
							}
							else {CloseHandle(readOv.hEvent); errorHandler("Error reading after waiting, GOR false."); return false;}
						}
						else {CloseHandle(readOv.hEvent); errorHandler("Error reading, wait for read failed."); return false;}						
					}
					else {CloseHandle(readOv.hEvent); errorHandler("Error reading."); return false;}					
				}
				else
				{
					lck.unlock();
					if(GetOverlappedResult(portHandle,&readOv,&bytesRead,false))
					{
						bufferReadData(dataInBuff,bytesRead);
					}
					else {CloseHandle(readOv.hEvent); errorHandler("Error reading, returned instantly but failed."); return false;}
				}
			}
			while(bytesRead>0);
		}
		else {CloseHandle(readOv.hEvent); errorHandler("Error reading, error waiting for char."); return false;}		
	}

	CloseHandle(readOv.hEvent);
	return true;
}

bool serialClass::serialThreads::bufferReadData(char* data, int byteNum)
{
	for(int i=0;i<=byteNum-1;i++)
	{
		readBuffer[rbwp]=data[i];
		if(rbwp>=(rbs-1)) rbwp=0; else rbwp++;
		readCue++;
	}

	return true;
}

bool serialClass::serialThreads::writeThreadFn()
{
	OVERLAPPED writeOv;
	memset(&writeOv,0,sizeof(writeOv));
	writeOv.hEvent=CreateEvent(NULL,true,false,NULL);
	if(writeOv.hEvent==NULL) {CloseHandle(writeOv.hEvent); errorHandler("Error writing creating ov."); return false;}

	while(writeThreadState)
	{
		if(writeCue)
		{
			char dataOutBuffer[tbs]={0};
			DWORD bytesWritten=0;

			int wc=writeCue;
			for(int i=0;i<=wc-1;i++)
			{
				dataOutBuffer[i]=writeBuffer[wbrp];
				if(wbrp>=tbs-1) wbrp=0; else wbrp++;
			}
			writeCue-=wc;

			bool state=false;
			unique_lock<mutex> lck(classMutex);								
			if(!WriteFile(portHandle,dataOutBuffer,wc,&bytesWritten,&writeOv))
			{
				DWORD lastError=GetLastError();
				lck.unlock();
				if(lastError==ERROR_IO_PENDING)
				{
					if(WaitForSingleObject(writeOv.hEvent,INFINITE)==WAIT_OBJECT_0)
					{						
						if(!GetOverlappedResult(portHandle,&writeOv,&bytesWritten,false)) {CloseHandle(writeOv.hEvent); errorHandler("Error writing, write failed after waiting."); return false;}
					}
					else {CloseHandle(writeOv.hEvent); errorHandler("Error writing, wait failed."); return false;}
				}
				else {CloseHandle(writeOv.hEvent); errorHandler("Error writing, write failed."); return false;}
			}
			else
			{
				lck.unlock();
				if(!GetOverlappedResult(portHandle,&writeOv,&bytesWritten,false)) {CloseHandle(writeOv.hEvent); errorHandler("Error write returned instantly, write failed."); return false;}
			}
		}
	}

	CloseHandle(writeOv.hEvent);
	return true;
}

serialClass::serialThreads threadClass;

bool serialClass::init(portSettings ps)
{
	portHandle=CreateFile
	(
		ps.comPort,
		GENERIC_READ | GENERIC_WRITE,
		0,
		0,
		OPEN_EXISTING,
		FILE_FLAG_OVERLAPPED,
		0
	);
	if(portHandle==INVALID_HANDLE_VALUE) {errorHandler("Invalid handle value"); return false;}

	DCB dcb={0};
	dcb.DCBlength=sizeof(DCB);

	if(!::GetCommState(portHandle,&dcb)) {errorHandler("Error getting comm state"); return false;}

	dcb.BaudRate=ps.baudRate;
	dcb.ByteSize=ps.byteSize;
	dcb.Parity=ps.parity;
	dcb.StopBits=ps.stopBits;
	dcb.fDtrControl=DTR_CONTROL_ENABLE;

	if(!::SetCommState(portHandle,&dcb)) {errorHandler("Error setting comm state"); return false;}

	COMMTIMEOUTS tOuts={0};

	tOuts.ReadIntervalTimeout=MAXDWORD;
	tOuts.ReadTotalTimeoutConstant=0;
	tOuts.ReadTotalTimeoutMultiplier=0;
	tOuts.WriteTotalTimeoutConstant=0;
	tOuts.WriteTotalTimeoutMultiplier=0;

	if(!SetCommTimeouts(portHandle,&tOuts)) {errorHandler("Error setting timeouts"); return false;}

	if(!SetCommMask(portHandle,EV_RXCHAR)) {errorHandler("Error setting comm mask"); return false;}
	
	threadClass.startThreads();
	if(!reset()) return false;
	return true;
}

void serialClass::end()
{
	CancelIoEx(portHandle,NULL);
	threadClass.endThreads();
    CloseHandle(portHandle);
}

bool serialClass::write(char* cstr)
{
	char buff[100]={0};
	strcpy_s(buff,sizeof(buff),cstr);
	strcat_s(buff,sizeof(buff),">");

	int i=0;
	while(buff[i]!=0) //Not suitable for raw data with 0 in it, only works with strings of text and such with null termination
	{
		writeBuffer[wbwp]=buff[i];
		if(wbwp>=(tbs-1)) wbwp=0; else wbwp++;
		threadClass.writeCue++;
		i++;
	}

	return true;
}

bool serialClass::reset()
{
	if(!EscapeCommFunction(portHandle,CLRDTR)) {errorHandler("Error clearing dtr"); return false;}
	if(!EscapeCommFunction(portHandle,SETDTR)) {errorHandler("Error setting dtr"); return false;}

	return true;
}

void serialClass::printBuffer()
{		
	while(threadClass.readCue>0)
	{
		cout<<readBuffer[rbrp];
		if(rbrp>=rbs-1) rbrp=0; else rbrp++;
		threadClass.readCue--;
	}
}

BOOL WINAPI CtrlHandler(DWORD fdwCtrlType)
{
	if(fdwCtrlType==CTRL_CLOSE_EVENT)
    {        
		serialClass sc;
		sc.end();		
        return TRUE;
    }
}

void errorHandler(string msg)
{
	cout<<"Error message: "<<msg<<endl;
	
    serialClass sc;
	sc.end();

    LPTSTR errorText=NULL;

	FormatMessage(
	FORMAT_MESSAGE_FROM_SYSTEM
	|FORMAT_MESSAGE_ALLOCATE_BUFFER
	|FORMAT_MESSAGE_IGNORE_INSERTS,
	NULL,
	GetLastError(),
	MAKELANGID(LANG_NEUTRAL,SUBLANG_DEFAULT),
	(LPTSTR) &errorText,
	0,
	NULL);

	cout<<"GetLastError: ";
	wcout<<errorText<<endl;
	LocalFree(errorText);
	errorText=NULL;
}

bool FindComPorts::listComPorts(wstring *portNameList,wstring *portList,int &numPorts,string &error)
{
	HKEY key;
	if(!openRegister(HKEY_LOCAL_MACHINE,L"HARDWARE\\DEVICEMAP\\SERIALCOMM",key)) {regErrorHandler(key,error,"Failed open register"); return false;}

	DWORD vNum=0;
	DWORD mLen=0;
	DWORD mSize=0;
	if(!infoKey(key,vNum,mLen,mSize)) {regErrorHandler(key,error,"Failed getting reg info"); return false;}
	if(vNum>MAX_PORT_NUM) {regErrorHandler(key,error,"Not enough mem"); return false;}
	if(!enumValueKey(key,vNum,mLen,mSize,portNameList,portList)) {regErrorHandler(key,error,"Error enum key"); return false;}

	numPorts=vNum;
	RegCloseKey(key);			

	return true;
}

void FindComPorts::regErrorHandler(HKEY key,string &error,const string errorCode)
{
	RegCloseKey(key);
	error=errorCode;
}

bool FindComPorts::openRegister(HKEY rootKey,const wchar_t* subKey,HKEY &opKey)
{
	LONG result=RegOpenKeyExW(rootKey,subKey,0,KEY_READ | KEY_QUERY_VALUE | KEY_WOW64_64KEY,&opKey);

	if(result!=ERROR_SUCCESS)
	{
		if(result==ERROR_FILE_NOT_FOUND) specificErrorCode="Key not found";
		specificErrorCode="Error opening key";

		return false;
	}
	else return true;
}

bool FindComPorts::infoKey(HKEY regKey,DWORD &numValues,DWORD &maxNameLen,DWORD &maxValueSize)
{
	LONG result=RegQueryInfoKeyW
	(
		regKey,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		&numValues,
		&maxNameLen,
		&maxValueSize,
		NULL,
		NULL
	);
	maxNameLen++; //To include null terminator
	if(result!=ERROR_SUCCESS)	
	{
		specificErrorCode="Error query info";
		return false;
	}
	else return true;
}

bool FindComPorts::enumValueKey(HKEY regKey,DWORD numValues,DWORD maxNameLen,DWORD maxValueSize,wstring *nameArray,wstring *valueArray)
{
	DWORD varType=REG_SZ;

	for(DWORD indexC=0;indexC<numValues;indexC++)
	{
		wchar_t *valueBuff=new wchar_t[maxValueSize/2];
		wchar_t *nameBuff=new wchar_t[maxNameLen];
		DWORD valueBuffSize=maxValueSize;
		DWORD nameBuffSize=maxNameLen;

		LONG result=RegEnumValueW
		(
			regKey,
			indexC,
			nameBuff,
			&nameBuffSize,
			NULL,
			&varType,
			(LPBYTE)valueBuff,
			&valueBuffSize
		);
		if(result!=ERROR_SUCCESS)
		{
			if(result==ERROR_NO_MORE_ITEMS)
			{
				specificErrorCode="Error no more items";
				return false;
			}
			else if(result==ERROR_MORE_DATA)
			{
				specificErrorCode="Error more data";
				return false;
			}
			else
			{
				specificErrorCode="Error enum";
				return false;
			}
		}
		else
		{
			nameArray[indexC]=nameBuff;
			valueArray[indexC]=valueBuff;
		}

		delete[] valueBuff;
		delete[] nameBuff;
	}

	return true;
}