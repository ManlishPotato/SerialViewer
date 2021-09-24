#pragma once
#include "wx/wx.h"
#include "settingsDialog.h"
#include "serialClass.h"

#include <thread>
#include <string>

#define baseID 10000
//TODO: unify IDs and events
//GUI events
const int btnConnectId=  baseID+1;
const int check1Id=	     baseID+2;
const int check2Id=	     baseID+3;
const int check3Id=      baseID+4;
const int settingsId=    baseID+5;
const int aboutId=       baseID+6;
const int cboxBaudId=    baseID+7;
const int updatePortsId= baseID+8;
const int cboxPortId=    baseID+9;
const int listSettingsId=baseID+10;
const int btnClearId=    baseID+11;

//Custom events
const int ptWriteEvtId= baseID2+12; //Print thread write read data evt
				   //scErrorEvt 13
const int txtWriteId=	 baseID+14;

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

		void updateComPorts();

		void printReadDataEvt(wxCommandEvent &evt);
		void serialThreadErrorEvt(wxCommandEvent &evt);

	private:
		void showErrorReport();
		
	public:
		wxTextCtrl* txtWrite=nullptr;
		wxTextCtrl* txtRead=nullptr;
		wxButton* btnConnect=nullptr;
		wxButton *btnClear=nullptr;
		wxComboBox* cbxBaud=nullptr;
		wxComboBox* cbxPort=nullptr;
		wxComboBox* cbxDelim=nullptr;
		wxCheckBox* chkReset=nullptr;
		wxCheckBox* chkScroll=nullptr;
		
		wxMenuBar* menubar;
		wxMenu* file;	
};

class DeafultPortSettings
{
	public:
		wxString comPort="COM3";
		wxString baudRate="9600";
		wxString byteSize="9 Bits";
		wxString parity="Even Parity";
		wxString stopBits="2 Stop Bits";
};