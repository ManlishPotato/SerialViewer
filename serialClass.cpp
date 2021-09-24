#include "serialClass.h"
using namespace std;

HANDLE portHandle; //Handle for the comm port

wxDECLARE_EVENT(SC_ERROR_EVT,wxCommandEvent);

void serialThreads::startThreads(wxEvtHandler *evtHandle)
{
	if(!readThreadState && !writeThreadState)
	{
		evtHandlePtr=evtHandle;
		readThreadState=true;
		writeThreadState=true;
		readThread=thread(&serialThreads::readThreadFn,this);
		writeThread=thread(&serialThreads::writeThreadFn,this);
	}
	//TODO: add error if else
}

void serialThreads::endThreads()
{
	if(readThreadState || writeThreadState)
	{
		readThreadState=false;
		writeThreadState=false;
		readThread.join();
		writeThread.join();
	}
	//TODO: add error if else unless in init
}

bool serialThreads::readThreadFn()
{		
	OVERLAPPED readOv={0};
	readOv.hEvent=CreateEvent(NULL,true,false,NULL);
	if(readOv.hEvent==NULL) {CloseHandle(readOv.hEvent); stErrorHandler("Error creating read event handle"); return false;}

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
							else {CloseHandle(readOv.hEvent); stErrorHandler("Error reading after waiting, GOR."); return false;}
						}
						else {CloseHandle(readOv.hEvent); stErrorHandler("Error reading, wait for read failed WFSO."); return false;}
					}
					else {CloseHandle(readOv.hEvent); stErrorHandler("Error reading GLE."); return false;}					
				}
				else
				{
					lck.unlock();
					if(GetOverlappedResult(portHandle,&readOv,&bytesRead,false))
					{
						bufferReadData(dataInBuff,bytesRead);
					}
					else {CloseHandle(readOv.hEvent); stErrorHandler("Error reading, returned instantly but failed."); return false;}
				}
			}
			while(bytesRead>0);
		}
		else {CloseHandle(readOv.hEvent); stErrorHandler("Error reading, error waiting for char."); return false;}		
	}

	CloseHandle(readOv.hEvent);
	return true;
}

bool serialThreads::bufferReadData(char* data, int byteNum)
{
	for(int i=0;i<=byteNum-1;i++)
	{
		readBuffer[rbwp]=data[i];
		if(rbwp>=(rbs-1)) rbwp=0; else rbwp++;
		readCue++;
	}

	return true;
}

bool serialThreads::writeThreadFn()
{
	OVERLAPPED writeOv;
	memset(&writeOv,0,sizeof(writeOv));
	writeOv.hEvent=CreateEvent(NULL,true,false,NULL);
	if(writeOv.hEvent==NULL) {CloseHandle(writeOv.hEvent); stErrorHandler("Error writing creating ov."); return false;}

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
						if(!GetOverlappedResult(portHandle,&writeOv,&bytesWritten,false)) {CloseHandle(writeOv.hEvent); stErrorHandler("Error writing, write failed after waiting GOR."); return false;}
					}
					else {CloseHandle(writeOv.hEvent); stErrorHandler("Error writing, wait failed WFSO."); return false;}
				}
				else {CloseHandle(writeOv.hEvent); stErrorHandler("Error writing, write failed GLE."); return false;}
			}
			else
			{
				lck.unlock();
				if(!GetOverlappedResult(portHandle,&writeOv,&bytesWritten,false)) {CloseHandle(writeOv.hEvent); stErrorHandler("Error write returned instantly, write failed."); return false;}
			}
		}
	}

	CloseHandle(writeOv.hEvent);
	return true;
}

void serialThreads::stErrorHandler(std::string msg)
{
	reportError(msg);
	wxCommandEvent *evt=new wxCommandEvent(SC_ERROR_EVT,stErrorEvtId);
	wxQueueEvent(evtHandlePtr,evt);	
}

void serialThreads::reportError(string msg)
{
	wchar_t *errorText=NULL;

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

	errorCode=msg;	
	spErrorCode=errorText;

	LocalFree(errorText);
	errorText=NULL;
}

bool serialClass::init(portSettings ps,wxEvtHandler *evtHandle)
{
	serialState=true;

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
	if(portHandle==INVALID_HANDLE_VALUE) {scErrorHandler("Invalid handle value"); return false;}

	DCB dcb={0};
	dcb.DCBlength=sizeof(DCB);

	if(!::GetCommState(portHandle,&dcb)) {scErrorHandler("Error getting comm state"); return false;}

	dcb.BaudRate=ps.baudRate;
	dcb.ByteSize=ps.byteSize;
	dcb.Parity=ps.parity;
	dcb.StopBits=ps.stopBits;
	dcb.fDtrControl=DTR_CONTROL_ENABLE;

	if(!::SetCommState(portHandle,&dcb)) {scErrorHandler("Error setting comm state"); return false;}

	COMMTIMEOUTS tOuts={0};

	tOuts.ReadIntervalTimeout=MAXDWORD;
	tOuts.ReadTotalTimeoutConstant=0;
	tOuts.ReadTotalTimeoutMultiplier=0;
	tOuts.WriteTotalTimeoutConstant=0;
	tOuts.WriteTotalTimeoutMultiplier=0;

	if(!SetCommTimeouts(portHandle,&tOuts)) {scErrorHandler("Error setting timeouts"); return false;}

	if(!SetCommMask(portHandle,EV_RXCHAR)) {scErrorHandler("Error setting comm mask"); return false;}
	
	startThreads(evtHandle);
	if(!reset()) return false;
	return true;
}

void serialClass::end()
{
	serialState=false;
	CancelIoEx(portHandle,NULL);
	endThreads();
    CloseHandle(portHandle);
}

bool serialClass::write(string str)
{
	char buff[tbs]={0};
	strcpy_s(buff,sizeof(buff),str.c_str());
	strcat_s(buff,sizeof(buff),">"); //TODO: change for delimiter setting

	int n=strlen(buff);
	for(int i=0;i<=n-1;i++)
	{
		writeBuffer[wbwp]=buff[i];
		if(wbwp>=(tbs-1)) wbwp=0; else wbwp++;
		writeCue++;
	}

	return true;
}

bool serialClass::reset()
{
	if(!EscapeCommFunction(portHandle,CLRDTR)) {scErrorHandler("Error clearing dtr"); return false;}
	if(!EscapeCommFunction(portHandle,SETDTR)) {scErrorHandler("Error setting dtr"); return false;}

	return true;
}

void serialClass::scErrorHandler(string msg)
{	    
	end();
	reportError(msg);
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