#pragma once
#include "wx/wx.h"
#include "settingsDialog.h"

class cMain : public wxFrame
{
	public:
		cMain();
		~cMain();

		void menuSettings(wxCommandEvent &evt);
		void menuAbout(wxCommandEvent &evt);
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

		settingsDialog* settingsDia=nullptr;		
};

