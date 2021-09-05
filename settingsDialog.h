#pragma once

#include <wx/wx.h>

class settingsDialog : public wxDialog
{

	public:
		settingsDialog(const wxString& title);

		void buttonOk(wxCommandEvent &evt);
		void buttonCancel(wxCommandEvent &evt);
		wxDECLARE_EVENT_TABLE();

};

