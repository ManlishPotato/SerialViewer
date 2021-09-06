#include "settingsDialog.h"

wxBEGIN_EVENT_TABLE(settingsDialog,wxDialog)
	EVT_BUTTON(wxID_EXIT,buttonCancel)
wxEND_EVENT_TABLE()

const int ch1Num=3;
const int ch2Num=3;
const int ch3Num=2;
wxString cbox1Choice[ch1Num]={"7 Bits","8 Bits","9 Bits"};
wxString cbox2Choice[ch2Num]={"No Parity","Even Parity","Odd Parity"};
wxString cbox3Choice[ch3Num]={"1 Stop Bit","2 Stop Bits"};

settingsDialog::settingsDialog(const wxString& title,wxString comboByteSize,wxString comboParity,wxString comboStopBits) : wxDialog(NULL,-1,title,wxDefaultPosition,wxSize(300,300))
{
	wxPanel* panel=new wxPanel(this,-1);

	cbox1=new wxComboBox(this,wxID_ANY,comboByteSize,wxPoint(20,20),wxSize(100,25),ch1Num,cbox1Choice);
	cbox2=new wxComboBox(this,wxID_ANY,comboParity,wxPoint(20,60),wxSize(100,25),ch2Num,cbox2Choice);
	cbox3=new wxComboBox(this,wxID_ANY,comboStopBits,wxPoint(20,100),wxSize(100,25),ch3Num,cbox3Choice);	

	btn1=new wxButton(this,wxID_OK,"OK",wxPoint(50,220),wxSize(75,30));
	btn2=new wxButton(this,wxID_EXIT,"Cancel",wxPoint(150,220),wxSize(75,30));
}

void settingsDialog::buttonCancel(wxCommandEvent &evt)
{
	EndModal(wxID_EXIT);
	evt.Skip();
}
