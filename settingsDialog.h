#pragma once

#include <wx/wx.h>

class settingsDialog : public wxDialog
{

	public:
		settingsDialog(const wxString& title);

		wxComboBox* cbox1=nullptr;
		wxComboBox* cbox2=nullptr;
		wxComboBox* cbox3=nullptr;

		wxButton* btn1=nullptr;
		wxButton* btn2=nullptr;

		void buttonOk(wxCommandEvent &evt);
		void buttonCancel(wxCommandEvent &evt);
		wxDECLARE_EVENT_TABLE();

	public:
		wxString comboParity="";
		wxString comboStopBits="";
		wxString comboByteSize="";

};

