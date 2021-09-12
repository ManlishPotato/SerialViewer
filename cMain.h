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
		void baudChange(wxCommandEvent &evt);
		wxDECLARE_EVENT_TABLE();
		
	public:
		wxTextCtrl* txt1=nullptr;
		wxTextCtrl* txt2=nullptr;
		wxButton* btn1=nullptr;
		wxComboBox* box1=nullptr;
		wxComboBox* box2=nullptr;
		wxComboBox* box3=nullptr;
		wxCheckBox* check1=nullptr;
		wxCheckBox* check2=nullptr;
		
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

