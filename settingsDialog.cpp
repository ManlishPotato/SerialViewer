#include "settingsDialog.h"

wxBEGIN_EVENT_TABLE(settingsDialog,wxDialog)
	EVT_BUTTON(wxID_EXIT,onBtnCancel)
wxEND_EVENT_TABLE()

settingsDialog::settingsDialog(const wxString& title,wxString comboByteSize,wxString comboParity,wxString comboStopBits,wxString comboDelim) : wxDialog(NULL,-1,title,wxDefaultPosition,wxSize(300,300))
{
	wxPanel* panel=new wxPanel(this,-1);

	cbxByteSize=new wxComboBox(this,wxID_ANY,comboByteSize,wxPoint(20,20),wxSize(100,25),byteSizeNum,byteSizeCho);
	cbxParity=new wxComboBox(this,wxID_ANY,comboParity,wxPoint(20,60),wxSize(100,25),parityNum,parityCho);
	cbxStopBits=new wxComboBox(this,wxID_ANY,comboStopBits,wxPoint(20,100),wxSize(100,25),stopBitsNum,stopBitsCho);
	cbxDelim=new wxComboBox(this,wxID_ANY,comboDelim,wxPoint(20,140),wxSize(100,25),delimNum,delimCho);

	btnOk=new wxButton(this,wxID_OK,"OK",wxPoint(50,220),wxSize(75,30));
	btnCancel=new wxButton(this,wxID_EXIT,"Cancel",wxPoint(150,220),wxSize(75,30));
}

void settingsDialog::onBtnCancel(wxCommandEvent &evt)
{
	EndModal(wxID_EXIT);
	evt.Skip();
}
