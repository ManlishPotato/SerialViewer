#include "settingsDialog.h"

settingsDialog::settingsDialog(const wxString& title,wxString comboByteSize,wxString comboParity,wxString comboStopBits,wxString comboDelim) : wxDialog(NULL,-1,title,wxDefaultPosition,wxSize(350,400))
{
	cbxByteSize=new wxComboBox(this,wxID_ANY,comboByteSize,wxDefaultPosition,wxSize(100,25),byteSizeNum,byteSizeCho,wxCB_READONLY);
	cbxParity=new wxComboBox(this,wxID_ANY,comboParity,wxDefaultPosition,wxSize(100,25),parityNum,parityCho,wxCB_READONLY);
	cbxStopBits=new wxComboBox(this,wxID_ANY,comboStopBits,wxDefaultPosition,wxSize(100,25),stopBitsNum,stopBitsCho,wxCB_READONLY);
	cbxDelim=new wxComboBox(this,wxID_ANY,comboDelim,wxDefaultPosition,wxSize(100,25),delimNum,delimCho,wxCB_READONLY);
	chkSave=new wxCheckBox(this,wxID_ANY,"Save preset",wxDefaultPosition,wxDefaultSize);	
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
	settSz->Add(chkSave,0,wxEXPAND | wxTOP | wxRIGHT,20);
	wxSizer *settAlignSz=new wxBoxSizer(wxVERTICAL);
	settAlignSz->Add(settSz,1,wxALIGN_LEFT);

	btnOk=new wxButton(this,btnOkId,"OK",wxDefaultPosition,wxDefaultSize);
	btnCancel=new wxButton(this,wxID_ANY,"Cancel",wxDefaultPosition,wxDefaultSize);
	wxSizer *controlSz=new wxBoxSizer(wxHORIZONTAL);
	controlSz->Add(btnOk,0,wxEXPAND | wxRIGHT,15);
	controlSz->Add(btnCancel,0,wxEXPAND | wxLEFT,15);
	wxSizer *controlAlignSz=new wxBoxSizer(wxHORIZONTAL);
	controlAlignSz->Add(controlSz,1,wxALIGN_BOTTOM,0);
	wxSizer *controlMarginSz=new wxBoxSizer(wxHORIZONTAL);
	controlMarginSz->Add(controlAlignSz,0,wxEXPAND | wxALL,10);

	wxBoxSizer *allSz=new wxBoxSizer(wxVERTICAL);
	allSz->Add(settAlignSz,1,wxEXPAND | wxLEFT,20);
	allSz->Add(controlMarginSz,1,wxALIGN_CENTER,0);

	this->SetSizer(allSz);	

	btnOk->Bind(wxEVT_BUTTON,&settingsDialog::onBtnOk,this,btnOkId);
	btnCancel->Bind(wxEVT_BUTTON,&settingsDialog::onBtnCancel,this);
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
