#include "settingsDialog.h"

int settBaseId=10000;
int settBtn1Id=settBaseId+7;
int settBtn2Id=settBaseId+8;

wxBEGIN_EVENT_TABLE(settingsDialog,wxDialog)
	EVT_BUTTON(settBtn1Id,buttonOk)
	EVT_BUTTON(settBtn2Id,buttonCancel)
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

	wxComboBox* cbox1=new wxComboBox(this,wxID_ANY,"No Parity",wxPoint(20,20),wxSize(100,25),ch1Num,cbox1Choice);
	wxComboBox* cbox2=new wxComboBox(this,wxID_ANY,"1 Stop Bit",wxPoint(20,60),wxSize(100,25),ch2Num,cbox2Choice);
	wxComboBox* cbox3=new wxComboBox(this,wxID_ANY,"8 Bits",wxPoint(20,100),wxSize(100,25),ch3Num,cbox3Choice);

	wxButton* btn1=new wxButton(this,settBtn1Id,"OK",wxPoint(50,220),wxSize(75,30));
	wxButton* btn2=new wxButton(this,settBtn2Id,"Cancel",wxPoint(150,220),wxSize(75,30));

	Centre();
	ShowModal();
	Destroy();
}

void settingsDialog::buttonOk(wxCommandEvent &evt)
{
	wxMessageBox(wxT("Test"));	

	evt.Skip();
}

void settingsDialog::buttonCancel(wxCommandEvent &evt)
{	
	evt.Skip();
}
