#pragma once
#include "wx/wx.h"
#include "settingsDialog.h"
#include "serialClass.h"

#include <thread>
#include <string>
#include <string.h>
#include "windows.h"

#define baudRateNum 2
const wxString baudRateCho[baudRateNum]={"9600","115200"};

#define baseID 10000
//TODO: unify IDs and events
const int btnConnectId=  baseID+1;
const int settingsId=    baseID+2;
const int aboutId=       baseID+3;
const int cboxBaudId=    baseID+4;
const int updatePortsId= baseID+5;
const int cboxPortId=    baseID+6;
const int listSettingsId=baseID+7;
const int btnClearId=    baseID+8;

const int ptWriteEvtId= baseID2+9; //Print thread write read data evt
		//stErrorEvt    baseID2+10;
const int txtWriteId=	 baseID+11;
const int chkScrollId=   baseID+12;

class printReadBuffer : public serialClass
{
	public:
		void startPrintThr(wxEvtHandler *evtHandle);
		void endPrintThr();

	private:
		void printThreadFn(wxEvtHandler *evtHandle);

	private:
		std::thread printThread;
		bool printThreadState=false;
};

class cMain : public wxFrame, public printReadBuffer
{
	public:
		cMain();
		~cMain();

		wxDECLARE_EVENT_TABLE();

		void menuSettings(wxCommandEvent &evt);
		void menuAbout(wxCommandEvent &evt);
		void menuUpdatePorts(wxCommandEvent &evt);
		void menuListSettings(wxCommandEvent &evt);

		void baudChange(wxCommandEvent &evt);
		void comPortChange(wxCommandEvent &evt);
		void serialConnect(wxCommandEvent &evt);
		void clearReadTxt(wxCommandEvent &evt);
		void onTxtWriteSend(wxCommandEvent &evt);
		void chkScrollChange(wxCommandEvent &evt);

		void updateComPorts();

		void printReadDataEvt(wxCommandEvent &evt);
		void serialThreadErrorEvt(wxCommandEvent &evt);

	private:
		void showErrorReport();
		void getUserSettings(portSettings &ps);
		
	public:
		wxTextCtrl* txtWrite=nullptr;
		wxTextCtrl* txtRead=nullptr;
		wxButton* btnConnect=nullptr;
		wxButton *btnClear=nullptr;
		wxComboBox* cbxBaud=nullptr;
		wxComboBox* cbxPort=nullptr;		
		wxCheckBox* chkReset=nullptr;
		wxCheckBox* chkScroll=nullptr;
		wxCheckBox *chkClear=nullptr;
		
		wxMenuBar* menubar;
		wxMenu* file;

	private:
		bool autoScroll=true;
};

class DeafultPortSettings
{
	public:
		wxString comPort="COM3";
		wxString baudRate="9600";
		wxString byteSize="8 bits";
		wxString parity="None";
		wxString stopBits="1 bit";
		wxString delim=">";
};