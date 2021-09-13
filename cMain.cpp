#include "cMain.h"
#include "windows.h"
using namespace std;

DeafultPortSettings dps;

int baseId=10000;
int btnConnectId=baseId+1;
int check1Id=baseId+2;
int check2Id=baseId+3;
int check3Id=baseId+4;
int settingsId=baseId+5;
int aboutId=baseId+6;
int cboxBaudId=baseId+7;
int updatePortsId=baseId+8;
int cboxPortId=baseId+9;

wxBEGIN_EVENT_TABLE(cMain,wxFrame)
	EVT_MENU(settingsId,menuSettings)
	EVT_MENU(aboutId,menuAbout)
	EVT_MENU(updatePortsId,menuUpdatePorts)
	EVT_COMBOBOX(cboxBaudId,baudChange)
	EVT_COMBOBOX(cboxPortId,comPortChange)
	EVT_BUTTON(btnConnectId,serialConnect)
wxEND_EVENT_TABLE()

const int boxChoiceNum=2;
const wxString box1Choice[boxChoiceNum]={"9600","115200"};
const wxString box3Choice[boxChoiceNum]={"Delim: /n/l","Delim: >"};

cMain::cMain() : wxFrame(nullptr,wxID_ANY,"SerialViewer1.0",wxPoint(30,30),wxSize(650,600))
{
	txtWrite=new wxTextCtrl(this,wxID_ANY,"",wxPoint(10,10),wxSize(500,28));
	txtRead=new wxTextCtrl(this,wxID_ANY,"",wxPoint(10,50),wxSize(500,450),wxTE_MULTILINE | wxTE_READONLY);
	btnConnect=new wxButton(this,btnConnectId,"Connect",wxPoint(520,10),wxSize(100,50));
	cbxBaud=new wxComboBox(this,cboxBaudId,dps.baudRate,wxPoint(515,90),wxSize(110,25),boxChoiceNum,box1Choice);
	cbxPort=new wxComboBox(this,cboxPortId,"",wxPoint(515,140),wxSize(110,25));
	cbxDelim=new wxComboBox(this,wxID_ANY,"Delim: /n/l",wxPoint(120,500),wxSize(100,25),boxChoiceNum,box3Choice);
	chkReset=new wxCheckBox(this,check1Id,"Auto DTR Reset",wxPoint(520,180),wxSize(100,25));
	chkScroll=new wxCheckBox(this,check2Id,"Auto Scroll",wxPoint(12,500),wxSize(100,25));

	updateComPorts();

	menubar=new wxMenuBar;
	file=new wxMenu;
	file->Append(settingsId,wxT("&Port settings"));
	file->Append(aboutId,wxT("&About"));
	file->Append(updatePortsId,wxT("Refresh Ports"));
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
	}	

	evt.Skip();
}

void cMain::menuAbout(wxCommandEvent &evt)
{
	wxMessageBox(wxT("SerialViewer version 1.0\nDevolped by Benjamin Solar."));

	evt.Skip();
}

void cMain::menuUpdatePorts(wxCommandEvent &evt)
{
	updateComPorts();

	evt.Skip();
}

void cMain::baudChange(wxCommandEvent &evt)
{
	dps.baudRate=cbxBaud->GetValue();	

	evt.Skip();
}

void cMain::comPortChange(wxCommandEvent &evt)
{
	dps.comPort=cbxPort->GetValue();

	evt.Skip();
}

void cMain::serialConnect(wxCommandEvent &evt)
{
	txtRead->AppendText("-Current settings-\n");
	txtRead->AppendText("Port: "+dps.comPort+"\n");
	txtRead->AppendText("Baud Rate: "+dps.baudRate+"\n");
	txtRead->AppendText("Byte Size: "+dps.byteSize+"\n");
	txtRead->AppendText("Parity: "+dps.parity+"\n");
	txtRead->AppendText("Stop Bits: "+dps.stopBits+"\n");
}

void cMain::updateComPorts()
{
	//TODO: Error code handler!!
	FindComPorts findCom;
	wstring ports[MAX_PORT_NUM];
	wstring portNames[MAX_PORT_NUM];
	int nPorts;
	string errorC;
	findCom.listComPorts(portNames,ports,nPorts,errorC);

	cbxPort->Clear();

	if(nPorts<1) 
	{
		cbxPort->Append("No COM ports");
		cbxPort->SetValue("No COM ports");
	}
	else
	{
		wxString portBuffer[MAX_PORT_NUM];
		for(int i=0;i<nPorts;i++) 
		{
			portBuffer[i]=ports[i];
			cbxPort->Append(portBuffer[i]);
		}
		cbxPort->SetValue(portBuffer[0]);
	}

	dps.comPort=cbxPort->GetValue();
}