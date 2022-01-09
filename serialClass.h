#pragma once

#include "wx/wx.h"
#include <iostream>
#include <windows.h>
#include <thread>
#include <chrono>
#include <mutex>
#include <string>
#include <string.h>

//Buffer size standards
#define rbs 255
#define tbs 255

class portSettings
{
	public:
		wchar_t comPort[32]=L"COM3";
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

		std::string errorCode="";
		std::wstring spErrorCode=L"";

	private:		
		std::thread readThread;
		std::thread writeThread;
		std::mutex classMutex;
		bool readThreadState=false;
		bool writeThreadState=false;
		wxEvtHandler *evtHandlePtr;	
		
	protected:
		HANDLE portHandle;
		const int scErrorEvtId=wxWindow::NewControlId();
};

class serialClass : public serialThreads
{	
	public:
		bool init(portSettings ps,wxEvtHandler *evtHandle,bool doResetMcu);
		void end();
		bool write(std::string str,char *delim);
		bool reset();
		
	private:
		void scErrorHandler(std::string msg); //Serial class error handler			
};

