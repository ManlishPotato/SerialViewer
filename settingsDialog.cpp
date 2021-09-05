#include "settingsDialog.h"

int settingsBaseId=10000;
int buttonOkId=settingsBaseId+7;

wxBEGIN_EVENT_TABLE(settingsDialog,wxDialog)
	EVT_BUTTON(buttonOkId,buttonOk)
	EVT_BUTTON(wxID_EXIT,buttonCancel)
wxEND_EVENT_TABLE()

const int ch1Num=3;
const int ch2Num=2;
const int ch3Num=3;
wxString cbox1Choice[ch1Num]={"No Parity","Even Parity","Odd Parity"};
wxString cbox2Choice[ch2Num]={"1 Stop Bit","2 Stop Bits"};
wxString cbox3Choice[ch3Num]={"7 Bits","8 Bits","9 Bits"};

settingsDialog::settingsDialog(const wxString& title) : wxDialog(NULL,-1,title,wxDefaultPosition,wxSize(300,300))
{
	wxPanel* panel=new wxPanel(this,-1);

	cbox1=new wxComboBox(this,wxID_ANY,"No Parity",wxPoint(20,20),wxSize(100,25),ch1Num,cbox1Choice);
	cbox2=new wxComboBox(this,wxID_ANY,"1 Stop Bit",wxPoint(20,60),wxSize(100,25),ch2Num,cbox2Choice);
	cbox3=new wxComboBox(this,wxID_ANY,"8 Bits",wxPoint(20,100),wxSize(100,25),ch3Num,cbox3Choice);

	btn1=new wxButton(this,buttonOkId,"OK",wxPoint(50,220),wxSize(75,30));
	btn2=new wxButton(this,wxID_EXIT,"Cancel",wxPoint(150,220),wxSize(75,30));

	Centre();
	ShowModal();	
}

void settingsDialog::buttonOk(wxCommandEvent &evt)
{
	comboParity=cbox1->GetValue();
	comboStopBits=cbox2->GetValue();
	comboByteSize=cbox3->GetValue();

	Close(true);
	evt.Skip();
}

void settingsDialog::buttonCancel(wxCommandEvent &evt)
{
	Close(true);
	evt.Skip();
}
