#pragma once
#include "wx/wx.h"
#include "settingsDialog.h"
#include "serialClass.h"
#include "SearchComPorts.h"

#include <thread>
#include <string>
#include <string.h>
#include "windows.h"

#define cbxBaudNum 2

class DeafultPortSettings
{
	public:
		wxString defComPort="COM3";
		wxString defBaudRate="9600";
		wxString defByteSize="8 bit";
		wxString defParity="None";
		wxString defStopBits="1 bit";
		wxString defDelim=">";
};

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

	protected:
		const int ptWriteEvtId=wxWindow::NewControlId();
};

class cMain : public wxFrame,public printReadBuffer,public DeafultPortSettings
{
	public:
		cMain();
		~cMain();		

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
		void uiConnected();
		void uiDisconnected();
		void uiError();
		
	public:
		const wxString cbxBaudSel[cbxBaudNum]={"9600","115200"};

		wxTextCtrl* txtWrite=nullptr;
		wxTextCtrl* txtRead=nullptr;
		wxButton* btnConnect=nullptr;
		wxButton *btnClear=nullptr;
		wxComboBox* cbxBaud=nullptr;
		wxComboBox* cbxPort=nullptr;		
		wxCheckBox* chkReset=nullptr;
		wxCheckBox* chkScroll=nullptr;
		wxCheckBox *chkClear=nullptr;
		wxStaticText *tlaConnected=nullptr;
		
		wxMenuBar* menubar;
		wxMenu* file;		

	private:
		bool autoScroll=true;

		wxFont font1=wxFont(12,wxFONTFAMILY_TELETYPE,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL);
		wxFont font2=wxFont(12,wxFONTFAMILY_TELETYPE,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL);

		wxColour secondaryColour1=wxColour(40,44,52); //Panel colour - dark 
		wxColour secondaryColour2=wxColour(64,72,89); //Text box colour - light dark
		wxColour txtColour1=*wxWHITE; //White text colour
		wxColour txtColour2=wxColour(170,170,170); //Dark/gray white text colour
		wxColour txtColour3=*wxGREEN; //Bright green text colour
		wxColour txtColour4=*wxRED; //Red text colour
};