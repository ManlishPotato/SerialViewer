#pragma once

#include <iostream>
#include <windows.h>
#include <thread>
#include <chrono>
#include <mutex>


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
				~serialThreads();

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
		serialClass();
		~serialClass();

		bool init(portSettings ps);
		void end();
		bool write(char* cstr);
		bool reset();
		void printBuffer();	
};

