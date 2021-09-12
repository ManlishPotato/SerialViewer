#pragma once

#include <iostream>
#include <windows.h>
#include <thread>
#include <chrono>
#include <mutex>
#include <string>
#include <string.h>

#define MAX_PORT_NUM 10


class serialClass
{
	public:
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
				serialThreads();				

				void startThreads();
				void endThreads();

			private:
				bool readThreadFn();
				bool bufferReadData(char* data, int byteNum);
				bool writeThreadFn();				

			public:
				int readCue=0;
				int writeCue=0;				

			private:
				std::thread readThread;
				std::thread writeThread;
				std::mutex classMutex;
				bool readThreadState=false;
				bool writeThreadState=false;
		};

	public:
		bool init(portSettings ps);
		void end();
		bool write(char* cstr);
		bool reset();
		void printBuffer();
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

