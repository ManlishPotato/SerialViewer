//TODO: Add debug for readCue and writeCue
#include "cMain.h"
#include "windows.h"
using namespace std;

DeafultPortSettings dps;

wxDECLARE_EVENT(PT_WRITE_EVT,wxCommandEvent); //Print thread write event
wxDECLARE_EVENT(SC_ERROR_EVT,wxCommandEvent); //Serial thread error event

wxDEFINE_EVENT(PT_WRITE_EVT,wxCommandEvent);
wxDEFINE_EVENT(SC_ERROR_EVT,wxCommandEvent);

wxBEGIN_EVENT_TABLE(cMain,wxFrame)
	EVT_MENU(settingsId,menuSettings)
	EVT_MENU(aboutId,menuAbout)
	EVT_MENU(updatePortsId,menuUpdatePorts)
	EVT_MENU(listSettingsId,menuListSettings)
	EVT_COMBOBOX(cboxBaudId,baudChange)
	EVT_COMBOBOX(cboxPortId,comPortChange)
	EVT_BUTTON(btnConnectId,serialConnect)
	EVT_BUTTON(btnClearId,clearReadTxt)
	EVT_TEXT_ENTER(txtWriteId,onTxtWriteSend)

	EVT_COMMAND(ptWriteEvtId,PT_WRITE_EVT,cMain::printReadDataEvt)	
	EVT_COMMAND(stErrorEvtId,SC_ERROR_EVT,cMain::serialThreadErrorEvt)
wxEND_EVENT_TABLE()

const int boxChoiceNum=2;
const wxString box1Choice[boxChoiceNum]={"9600","115200"};
const wxString box3Choice[boxChoiceNum]={"Delim: /n/l","Delim: >"};

cMain::cMain() : wxFrame(nullptr,wxID_ANY,"SerialViewer1.0",wxPoint(30,30),wxSize(650,600))
{
	txtWrite=new wxTextCtrl(this,txtWriteId,"",wxPoint(10,10),wxSize(500,28),wxTE_PROCESS_ENTER);
	txtRead=new wxTextCtrl(this,wxID_ANY,"",wxPoint(10,50),wxSize(500,450),wxTE_MULTILINE | wxTE_READONLY);
	btnConnect=new wxButton(this,btnConnectId,"Connect",wxPoint(520,10),wxSize(100,50));
	btnClear=new wxButton(this,btnClearId,"Clear",wxPoint(250,505),wxSize(100,25));
	cbxBaud=new wxComboBox(this,cboxBaudId,dps.baudRate,wxPoint(515,90),wxSize(110,25),boxChoiceNum,box1Choice);
	cbxPort=new wxComboBox(this,cboxPortId,"",wxPoint(515,140),wxSize(110,25));
	cbxDelim=new wxComboBox(this,wxID_ANY,"Delim: /n/l",wxPoint(120,505),wxSize(100,25),boxChoiceNum,box3Choice);
	chkReset=new wxCheckBox(this,check1Id,"Auto DTR Reset",wxPoint(520,180),wxSize(100,25));
	chkScroll=new wxCheckBox(this,check2Id,"Auto Scroll",wxPoint(12,505),wxSize(100,25));

	updateComPorts();

	menubar=new wxMenuBar;
	file=new wxMenu;
	file->Append(settingsId,wxT("&Port settings"));
	file->Append(aboutId,wxT("&About"));
	file->Append(updatePortsId,wxT("Refresh Ports"));
	file->Append(listSettingsId,wxT("List Settings"));
	menubar->Append(file,wxT("&File"));
	SetMenuBar(menubar);	
}

cMain::~cMain()
{
	end();
	endPrintThr();
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

void cMain::menuListSettings(wxCommandEvent &evt)
{
	txtRead->AppendText("-Current settings-\n");
	txtRead->AppendText("Port: "+dps.comPort+"\n");
	txtRead->AppendText("Baud Rate: "+dps.baudRate+"\n");
	txtRead->AppendText("Byte Size: "+dps.byteSize+"\n");
	txtRead->AppendText("Parity: "+dps.parity+"\n");
	txtRead->AppendText("Stop Bits: "+dps.stopBits+"\n");

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
	portSettings defaultSettings; //Needs to be changed to user settings

	serialState=!serialState;
	if(serialState)
	{
		//Init serial
		btnConnect->SetLabel("Connected");
		if(!init(defaultSettings,this))
		{
			//Error at init
			showErrorReport();
			btnConnect->SetLabel("Disconnected");	
		}
		else
		{
			//Otherwise start printing thread
			startPrintThr(this);
		}
	}
	else
	{
		//Otherwise end serial
		//TODO: flush serial buffer before close
		btnConnect->SetLabel("Disconnected");
		end();
		endPrintThr();
	}

	evt.Skip();
}

void cMain::clearReadTxt(wxCommandEvent &evt)
{
	txtRead->Clear();

	evt.Skip();
}

void cMain::onTxtWriteSend(wxCommandEvent &evt)
{
	string str; //May be removed later
	str=txtWrite->GetValue();
	txtWrite->Clear();

	write(str);

	//Absence of evt.Skip() removes the warning bell sound on windows when pressing enter
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
		cbxPort->Append("No com. ports"); //Add value to list
		cbxPort->SetValue("No com. ports"); //Select added value for display
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

void cMain::printReadDataEvt(wxCommandEvent &evt)
{
	//Print buffer
	txtRead->AppendText(evt.GetString());
}

void cMain::serialThreadErrorEvt(wxCommandEvent &evt)
{	
	//Handle runtime error
	serialState=false;
	end();
	showErrorReport();
	btnConnect->SetLabel("Disconnected");	
}

void cMain::showErrorReport()
{
	txtRead->AppendText("\n\nError: "+errorCode+'\n');
	txtRead->AppendText("Specific error: "+spErrorCode+'\n');
}

void printReadBuffer::startPrintThr(wxEvtHandler *evtHandle)
{
	if(printThreadState==false)
	{
		printThreadState=true;
		printThread=thread(&printReadBuffer::printThreadFn,this,evtHandle);
	}
}

void printReadBuffer::endPrintThr()
{
	if(printThreadState)
	{
		printThreadState=false;
		printThread.join();
	}
}

void printReadBuffer::printThreadFn(wxEvtHandler* evtHandle)
{
	while(printThreadState)
	{
		if(readCue)
		{
			int cue=readCue;
			char buff[rbs+1]={0};
			string str; //May be remove later
			int offset=0; //To accuount for /0 in buffer
			//TODO: slow read with /r and /0 detection, should be removed in future
			for(int i=0;i<=cue-1;i++)
			{
				if(readBuffer[rbrp]==0 || readBuffer[rbrp]=='\r') offset++; //Add to offset if there is a 0
				else buff[i-offset]=readBuffer[rbrp];
				if(rbrp>=rbs-1) rbrp=0; else rbrp++;		
			}
			readCue-=cue;
			buff[cue-offset]='\0'; //Add null terminator
			str=buff;

			wxCommandEvent *evt=new wxCommandEvent(PT_WRITE_EVT,ptWriteEvtId);
			evt->SetString(str);
			wxQueueEvent(evtHandle,evt);
			//TODO: add debug for number of queued events
		}
	}
}	