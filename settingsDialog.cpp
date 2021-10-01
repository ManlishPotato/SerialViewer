#include "settingsDialog.h"

wxBEGIN_EVENT_TABLE(settingsDialog,wxDialog)
	EVT_BUTTON(btnOkId,onBtnOk)
	EVT_BUTTON(wxID_EXIT,onBtnCancel)	
wxEND_EVENT_TABLE()

settingsDialog::settingsDialog(const wxString& title,wxString comboByteSize,wxString comboParity,wxString comboStopBits,wxString comboDelim) : wxDialog(NULL,-1,title,wxDefaultPosition,wxDefaultSize)
{
	cbxByteSize=new wxComboBox(this,wxID_ANY,comboByteSize,wxDefaultPosition,wxSize(100,25),byteSizeNum,byteSizeCho,wxCB_READONLY);
	cbxParity=new wxComboBox(this,wxID_ANY,comboParity,wxDefaultPosition,wxSize(100,25),parityNum,parityCho,wxCB_READONLY);
	cbxStopBits=new wxComboBox(this,wxID_ANY,comboStopBits,wxDefaultPosition,wxSize(100,25),stopBitsNum,stopBitsCho,wxCB_READONLY);
	cbxDelim=new wxComboBox(this,wxID_ANY,comboDelim,wxDefaultPosition,wxSize(100,25),delimNum,delimCho,wxCB_READONLY);
	wxStaticText *stxByteSize=new wxStaticText(this,wxID_ANY,"Byte Size",wxDefaultPosition,wxDefaultSize);
	wxStaticText *stxParity=new wxStaticText(this,wxID_ANY,"Parity",wxDefaultPosition,wxDefaultSize);
	wxStaticText *stxStopBits=new wxStaticText(this,wxID_ANY,"Stop Bits",wxDefaultPosition,wxDefaultSize);
	wxStaticText *stxDelim=new wxStaticText(this,wxID_ANY,"Delimiter",wxDefaultPosition,wxDefaultSize);
	wxSizer *settSz=new wxBoxSizer(wxVERTICAL);
	settSz->Add(stxByteSize,0,wxEXPAND | wxTOP,20);
	settSz->Add(cbxByteSize,0,wxEXPAND | wxBOTTOM | wxRIGHT,10);
	settSz->Add(stxParity,0,wxEXPAND,0);
	settSz->Add(cbxParity,0,wxEXPAND | wxBOTTOM | wxRIGHT,10);
	settSz->Add(stxStopBits,0,wxEXPAND ,0);
	settSz->Add(cbxStopBits,0,wxEXPAND | wxBOTTOM | wxRIGHT,10);
	settSz->Add(stxDelim,0,wxEXPAND,0);
	settSz->Add(cbxDelim,0,wxEXPAND | wxRIGHT,10);
	wxSizer *settAlignSz=new wxBoxSizer(wxVERTICAL);
	settAlignSz->Add(settSz,1,wxALIGN_LEFT);

	btnOk=new wxButton(this,btnOkId,"OK",wxDefaultPosition,wxDefaultSize);
	btnCancel=new wxButton(this,wxID_EXIT,"Cancel",wxDefaultPosition,wxDefaultSize);
	wxSizer *controlSz=new wxBoxSizer(wxHORIZONTAL);
	controlSz->Add(btnOk,0,wxEXPAND | wxTOP | wxRIGHT | wxBOTTOM,10);
	controlSz->Add(btnCancel,0,wxEXPAND | wxTOP | wxLEFT | wxBOTTOM,10);
	wxSizer *controlAlignSz=new wxBoxSizer(wxHORIZONTAL);
	controlAlignSz->Add(controlSz,1,wxALIGN_BOTTOM,0);
	wxSizer *controlMarginSz=new wxBoxSizer(wxHORIZONTAL);
	controlMarginSz->Add(controlAlignSz,0,wxEXPAND | wxLEFT | wxRIGHT,40);

	wxBoxSizer *allSz=new wxBoxSizer(wxVERTICAL);
	allSz->Add(settAlignSz,2,wxEXPAND | wxLEFT,20);
	allSz->Add(controlMarginSz,1,wxALIGN_CENTER,0);

	this->SetSizerAndFit(allSz);
}

void settingsDialog::onBtnOk(wxCommandEvent &evt)
{
	EndModal(btnOkId);

	evt.Skip();
}

void settingsDialog::onBtnCancel(wxCommandEvent &evt)
{
	EndModal(wxID_EXIT);

	evt.Skip();
}
