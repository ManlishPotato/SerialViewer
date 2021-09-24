#pragma once

#include "wx/wx.h"
#include <iostream>
#include <windows.h>
#include <thread>
#include <chrono>
#include <mutex>
#include <string>
#include <string.h>

//Custom events
#define baseID2 10000
const int stErrorEvtId=  baseID2+13; //Serial thread error throw evt

//Buffer size standards
#define rbs 256
#define tbs 256
#define MAX_PORT_NUM 10

class portSettings
{
	public:
		const wchar_t* comPort=L"COM3";
		DWORD baudRate=9600;
		BYTE byteSize=8;
		BYTE parity=NOPARITY;
		BYTE stopBits=ONESTOPBIT;
};

class serialThreads
{
	public:
		void startThreads(wxEvtHandler *evtHandle);
		void endThreads();		

	private:		
		bool readThreadFn();
		bool bufferReadData(char* data, int byteNum);
		bool writeThreadFn();
		void stErrorHandler(std::string msg); //serial threads error handler
		
	protected:
		void reportError(std::string msg);

	public:
		bool serialState=false;

		//Read buffer
		char readBuffer[rbs]={0};
		int rbrp=0;
		int rbwp=0;
		int readCue=0;		

		//Write buffer
		char writeBuffer[tbs]={0};
		int wbrp=0;
		int wbwp=0;
		int writeCue=0;

		std::string errorCode;
		std::wstring spErrorCode;

	private:
		std::thread readThread;
		std::thread writeThread;
		std::mutex classMutex;
		bool readThreadState=false;
		bool writeThreadState=false;
		wxEvtHandler *evtHandlePtr;
};

class serialClass : public serialThreads
{	
	public:
		bool init(portSettings ps,wxEvtHandler *evtHandle);
		void end();
		bool write(std::string str);
		bool reset();
		
	private:
		void scErrorHandler(std::string msg); //Serial class error handler
};

class FindComPorts
{
	public:
		bool listComPorts(std::wstring *portNameList,std::wstring *portList,int &numPorts,std::string &error);

	private:
		void regErrorHandler(HKEY key,std::string &error,const std::string errorCode);
		bool openRegister(HKEY rootKey,const wchar_t* subKey,HKEY &opKey);
		bool infoKey(HKEY regKey,DWORD &numValues,DWORD &maxNameLen,DWORD &maxValueSize);
		bool enumValueKey(HKEY regKey,DWORD numValues,DWORD maxNameLen,DWORD maxValueSize,std::wstring *nameArray,std::wstring *valueArray);

	public:
		std::string specificErrorCode;
};

