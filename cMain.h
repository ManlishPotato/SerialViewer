#pragma once
#include "wx/wx.h"
#include "settingsDialog.h"
#include "serialClass.h";

class cMain : public wxFrame
{
	public:
		cMain();
		~cMain();

		void menuSettings(wxCommandEvent &evt);
		void menuAbout(wxCommandEvent &evt);
		void menuUpdatePorts(wxCommandEvent &evt);
		void baudChange(wxCommandEvent &evt);
		void comPortChange(wxCommandEvent &evt);
		void serialConnect(wxCommandEvent &evt);
		void updateComPorts();		

		wxDECLARE_EVENT_TABLE();
		
	public:
		wxTextCtrl* txtWrite=nullptr;
		wxTextCtrl* txtRead=nullptr;
		wxButton* btnConnect=nullptr;
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

