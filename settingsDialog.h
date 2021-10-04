#pragma once

#include <wx/wx.h>

class cbxPortSettings
{
	#define byteSizeNum 2
	#define parityNum 3
	#define stopBitsNum 2
	#define delimNum 2

	public:
		const wxString byteSizeCho[byteSizeNum]={"7 bit","8 bit"};
		const wxString parityCho[parityNum]={"None","Even","Odd"};
		const wxString stopBitsCho[stopBitsNum]={"1 bit","2 bit"};
		const wxString delimCho[delimNum]={">","\\n\\r"};
};

class settingsDialog : public wxDialog,public cbxPortSettings
{
	public:
		settingsDialog(const wxString& title,wxString comboByteSize,wxString comboParity,wxString comboStopBits,wxString comboDelim);		
				
		void onBtnOk(wxCommandEvent &evt);
		void onBtnCancel(wxCommandEvent &evt);		

	public:
		const int btnOkId=wxWindow::NewControlId();

		wxComboBox* cbxByteSize=nullptr;
		wxComboBox* cbxParity=nullptr;
		wxComboBox* cbxStopBits=nullptr;
		wxComboBox* cbxDelim=nullptr;
		wxCheckBox *chkSave=nullptr;

		wxButton* btnOk=nullptr;
		wxButton* btnCancel=nullptr;
};

