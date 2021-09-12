#pragma once

#include <wx/wx.h>

class settingsDialog : public wxDialog
{
	public:
		settingsDialog(const wxString& title,wxString comboByteSize,wxString comboParity,wxString comboStopBits);		

		wxComboBox* cbox1=nullptr;
		wxComboBox* cbox2=nullptr;
		wxComboBox* cbox3=nullptr;

		wxButton* btn1=nullptr;
		wxButton* btn2=nullptr;
				
		void buttonCancel(wxCommandEvent &evt);
		wxDECLARE_EVENT_TABLE();
};

