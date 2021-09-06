#include "cMain.h"
#include "windows.h"

DeafultPortSettings dps;

int baseId=10000;
int btn1Id=baseId+1;
int check1Id=baseId+2;
int check2Id=baseId+3;
int check3Id=baseId+4;
int settingsId=baseId+5;
int aboutId=baseId+6;

wxBEGIN_EVENT_TABLE(cMain,wxFrame)
	EVT_MENU(settingsId,menuSettings)
	EVT_MENU(aboutId,menuAbout)
wxEND_EVENT_TABLE()

const int boxChoiceNum=2;
const wxString box1Choice[boxChoiceNum]={"9600","115200"};
const wxString box2Choice[boxChoiceNum]={"COM3","COM4"};
const wxString box3Choice[boxChoiceNum]={"Delim: /n/l","Delim: >"};

cMain::cMain() : wxFrame(nullptr,wxID_ANY,"SerialViewer1.0",wxPoint(30,30),wxSize(650,600))
{
	txt1=new wxTextCtrl(this,wxID_ANY,"",wxPoint(10,10),wxSize(500,28));
	txt2=new wxTextCtrl(this,wxID_ANY,"",wxPoint(10,50),wxSize(500,450),wxTE_MULTILINE | wxTE_READONLY);
	btn1=new wxButton(this,btn1Id,"Connect",wxPoint(520,10),wxSize(100,50));
	box1=new wxComboBox(this,wxID_ANY,"9600",wxPoint(520,90),wxSize(100,25),boxChoiceNum,box1Choice);
	box2=new wxComboBox(this,wxID_ANY,"COM4",wxPoint(520,140),wxSize(100,25),boxChoiceNum,box2Choice);
	box3=new wxComboBox(this,wxID_ANY,"Delim: /n/l",wxPoint(120,500),wxSize(100,25),boxChoiceNum,box3Choice);
	check1=new wxCheckBox(this,check1Id,"Auto DTR Reset",wxPoint(520,180),wxSize(100,25));
	check2=new wxCheckBox(this,check2Id,"Auto Scroll",wxPoint(12,500),wxSize(100,25));

	menubar=new wxMenuBar;
	file=new wxMenu;
	file->Append(settingsId,wxT("&Port settings"));
	file->Append(aboutId,wxT("&About"));
	menubar->Append(file,wxT("&File"));
	SetMenuBar(menubar);	
}

cMain::~cMain()
{

}

void cMain::menuSettings(wxCommandEvent &evt)
{	
	settingsDialog sd(wxT("Port Settings"),dps.byteSize,dps.parity,dps.stopBits);
	if(sd.ShowModal()==wxID_OK)
	{
		dps.byteSize=sd.cbox1->GetValue();
		dps.parity=sd.cbox2->GetValue();
		dps.stopBits=sd.cbox3->GetValue();

		txt2->AppendText(dps.byteSize+"\n");
		txt2->AppendText(dps.parity+"\n");
		txt2->AppendText(dps.stopBits+"\n");
	}	

	evt.Skip();
}

void cMain::menuAbout(wxCommandEvent &evt)
{
	wxMessageBox(wxT("SerialViewer version 1.0\nDevolped by Benjamin Solar."));

	evt.Skip();
}