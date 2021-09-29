//TODO: Add debug for readCue and writeCue
#include "cMain.h"
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
	EVT_CHECKBOX(chkScrollId,chkScrollChange)

	EVT_COMMAND(ptWriteEvtId,PT_WRITE_EVT,cMain::printReadDataEvt)	
	EVT_COMMAND(stErrorEvtId,SC_ERROR_EVT,cMain::serialThreadErrorEvt)
wxEND_EVENT_TABLE()

cMain::cMain() : wxFrame(nullptr,wxID_ANY,"SerialViewer2.0",wxPoint(30,30),wxSize(650,600))
{			
	wxPanel *topBar=new wxPanel(this,wxID_ANY,wxDefaultPosition,wxDefaultSize);
	topBar->SetBackgroundColour(secondaryColour1);
	wxPanel *mainPanel=new wxPanel(this,wxID_ANY,wxDefaultPosition,wxDefaultSize);
	mainPanel->SetBackgroundColour(secondaryColour1);
	wxPanel *sideBar=new wxPanel(this,wxID_ANY,wxDefaultPosition,wxDefaultSize);
	sideBar->SetBackgroundColour(secondaryColour1);
	wxPanel *bottomBar=new wxPanel(this,wxID_ANY,wxDefaultPosition,wxDefaultSize);
	bottomBar->SetBackgroundColour(secondaryColour1);

	wxSizer *leftGroupSz=new wxBoxSizer(wxVERTICAL);
	leftGroupSz->Add(topBar,0,wxEXPAND | wxALL,5);
	leftGroupSz->Add(mainPanel,1,wxEXPAND | wxLEFT | wxBOTTOM | wxRIGHT,5);

	wxSizer *mainGroupSz=new wxBoxSizer(wxHORIZONTAL);
	mainGroupSz->Add(leftGroupSz,1,wxEXPAND,0);
	mainGroupSz->Add(sideBar,0,wxEXPAND | wxTOP | wxRIGHT | wxBOTTOM,5);

	wxBoxSizer *allSz=new wxBoxSizer(wxVERTICAL);
	allSz->Add(mainGroupSz,1,wxEXPAND,0);
	allSz->Add(bottomBar,0,wxEXPAND | wxLEFT | wxBOTTOM | wxRIGHT,5);
	
	txtWrite=new wxTextCtrl(topBar,txtWriteId,"",wxDefaultPosition,wxDefaultSize,wxTE_PROCESS_ENTER);
	txtWrite->SetBackgroundColour(secondaryColour2);
	txtWrite->SetForegroundColour(txtColour1);
	wxSizer *txtWriteSz=new wxBoxSizer(wxVERTICAL);
	txtWriteSz->Add(txtWrite,0,wxEXPAND | wxTOP | wxBOTTOM,5);
	topBar->SetSizerAndFit(txtWriteSz);

	txtRead=new wxTextCtrl(mainPanel,wxID_ANY,"",wxDefaultPosition,wxSize(500,450),wxTE_MULTILINE | wxTE_READONLY);
	txtRead->SetBackgroundColour(secondaryColour2);
	txtRead->SetForegroundColour(txtColour1);
	wxSizer *txtReadSz=new wxBoxSizer(wxVERTICAL);
	txtReadSz->Add(txtRead,1,wxEXPAND,0);
	mainPanel->SetSizer(txtReadSz);

	btnConnect=new wxButton(sideBar,btnConnectId,"Connect",wxDefaultPosition,wxSize(100,50));
	cbxBaud=new wxComboBox(sideBar,cboxBaudId,dps.baudRate,wxDefaultPosition,wxSize(110,25),baudRateNum,baudRateCho,wxCB_READONLY);
	cbxPort=new wxComboBox(sideBar,cboxPortId,"",wxDefaultPosition,wxSize(110,25));
	chkReset=new wxCheckBox(sideBar,wxID_ANY,"Auto DTR Reset",wxDefaultPosition,wxSize(100,25));
	chkReset->SetForegroundColour(txtColour1);	
	wxSizer *mainSettSz=new wxBoxSizer(wxVERTICAL);
	mainSettSz->Add(btnConnect,0,wxEXPAND | wxBOTTOM,10);
	mainSettSz->Add(cbxBaud,0,wxEXPAND | wxTOP | wxBOTTOM,10);
	mainSettSz->Add(cbxPort,0,wxEXPAND | wxTOP | wxBOTTOM,10);
	mainSettSz->Add(chkReset,0,wxEXPAND | wxTOP | wxBOTTOM,10);

	tlaConnected=new wxStaticText(sideBar,tlaConnectedId,"Unconnected",wxDefaultPosition,wxDefaultSize,wxALIGN_CENTER_HORIZONTAL);
	tlaConnected->SetForegroundColour(txtColour2);
	wxSizer *tlaConnectedSz=new wxBoxSizer(wxVERTICAL);
	tlaConnectedSz->Add(tlaConnected,0,wxEXPAND | wxALL,5);
	wxSizer *tlaConnectedAlignSz=new wxBoxSizer(wxHORIZONTAL);
	tlaConnectedAlignSz->Add(tlaConnectedSz,1,wxALIGN_BOTTOM,0);

	wxSizer *sideBarSz=new wxBoxSizer(wxVERTICAL);
	sideBarSz->Add(mainSettSz,1,wxEXPAND | wxALL,5);
	sideBarSz->Add(tlaConnectedAlignSz,1,wxALIGN_CENTER,0);
	sideBar->SetSizerAndFit(sideBarSz);

	btnClear=new wxButton(bottomBar,btnClearId,"Clear",wxDefaultPosition,wxSize(100,25));	
	chkScroll=new wxCheckBox(bottomBar,chkScrollId,"Auto Scroll",wxDefaultPosition,wxSize(80,25));
	chkClear=new wxCheckBox(bottomBar,wxID_ANY,"Auto Clear",wxDefaultPosition,wxSize(80,25));
	chkScroll->SetForegroundColour(txtColour1);
	chkClear->SetForegroundColour(txtColour1);
	wxSizer *otherSettSz=new wxBoxSizer(wxHORIZONTAL);
	otherSettSz->Add(btnClear,0,wxEXPAND | wxALL,5);
	otherSettSz->Add(chkScroll,0,wxEXPAND | wxALL,5);
	otherSettSz->Add(chkClear,0,wxEXPAND | wxALL,5);

	wxSizer *bottomBarSz=new wxBoxSizer(wxHORIZONTAL);
	bottomBarSz->Add(otherSettSz,1,wxALIGN_LEFT,0);
	bottomBar->SetSizerAndFit(bottomBarSz);	

	this->SetSizer(allSz);
	this->SetMinSize(wxSize(400,300));	

	chkReset->SetValue(true);
	chkScroll->SetValue(true);
	chkClear->SetValue(true);

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
	settingsDialog sd(wxT("Port Settings"),dps.byteSize,dps.parity,dps.stopBits,dps.delim);
	if(sd.ShowModal()==wxID_OK)
	{
		dps.byteSize=sd.cbxByteSize->GetValue();
		dps.parity=sd.cbxParity->GetValue();
		dps.stopBits=sd.cbxStopBits->GetValue();
		dps.delim=sd.cbxDelim->GetValue();
	}	

	evt.Skip();
}

void cMain::menuAbout(wxCommandEvent &evt)
{
	wxMessageBox(wxT("SerialViewer version 2.0\nDeveloped by Benjamin Solar."));

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
	txtRead->AppendText("Port: "+dps.comPort+'\n');
	txtRead->AppendText("Baud Rate: "+dps.baudRate+'\n');
	txtRead->AppendText("Byte Size: "+dps.byteSize+'\n');
	txtRead->AppendText("Parity: "+dps.parity+'\n');
	txtRead->AppendText("Stop Bits: "+dps.stopBits+'\n');
	txtRead->AppendText("Delimiter: "+dps.delim+'\n');

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
	portSettings pSettings;
	getUserSettings(pSettings);

	serialState=!serialState;
	if(serialState)
	{
		//Init serial		
		if(!init(pSettings,this,chkReset->GetValue()))
		{
			//Error at init
			showErrorReport();
			uiError();
		}
		else
		{
			//Otherwise start printing thread
			uiConnected();
			startPrintThr(this);
			if(chkClear->GetValue()) txtRead->Clear();
		}
	}
	else
	{
		//Otherwise end serial
		//TODO: flush serial buffer before close
		uiDisconnected();
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
	string dataWrite; 
	dataWrite=txtWrite->GetValue();
	txtWrite->Clear();

	char delimBuff[10];
	strcpy_s(delimBuff,sizeof(delimBuff),dps.delim.c_str());
	write(dataWrite,delimBuff);

	//Absence of evt.Skip() removes the warning bell sound on windows when pressing enter
}

void cMain::chkScrollChange(wxCommandEvent &evt)
{
	autoScroll=chkScroll->GetValue();

	evt.Skip();
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
	if(autoScroll)
	{
		txtRead->AppendText(evt.GetString());
	}
	else
	{
		long pos = ::SendMessage( txtRead->GetHWND(), EM_GETFIRSTVISIBLELINE, 0, 0 );

		txtRead->AppendText(evt.GetString());

		int newpos = ::SendMessage( txtRead->GetHWND(), EM_GETFIRSTVISIBLELINE, 0, 0 );
		::SendMessage( txtRead->GetHWND(), EM_LINESCROLL, 0, pos-newpos );
	}
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

void cMain::getUserSettings(portSettings &ps)
{	
	int sizeDest=sizeof(ps.comPort);
	sizeDest/=2; //For number of wide chars
	wcscpy_s(ps.comPort,sizeDest,dps.comPort.wc_str());

	if(dps.baudRate==baudRateCho[0]) ps.baudRate=9600;
	else if(dps.baudRate==baudRateCho[1]) ps.baudRate=115200;

	if(dps.byteSize==byteSizeCho[0]) ps.byteSize=7;
	else if(dps.byteSize==byteSizeCho[1]) ps.byteSize=8;	

	if(dps.parity==parityCho[0]) ps.parity=NOPARITY;
	else if(dps.parity==parityCho[1]) ps.parity=EVENPARITY;
	else if(dps.parity==parityCho[2]) ps.parity=ODDPARITY;

	if(dps.stopBits==stopBitsCho[0]) ps.stopBits=ONESTOPBIT;
	else if(dps.stopBits==stopBitsCho[1]) ps.stopBits=TWOSTOPBITS;
}

void cMain::uiConnected()
{
	btnConnect->SetLabel("Disconnect");

	tlaConnected->SetLabel("Connected");
	tlaConnected->SetForegroundColour(txtColour3);

	//TODO: Disable menu items
	cbxPort->Enable(false);
	cbxBaud->Enable(false);
	chkReset->Enable(false);
	chkClear->Enable(false);
}

void cMain::uiDisconnected()
{
	btnConnect->SetLabel("Connect");

	tlaConnected->SetLabel("Disconnected");
	tlaConnected->SetForegroundColour(txtColour2);

	cbxPort->Enable(true);
	cbxBaud->Enable(true);
	chkReset->Enable(true);
	chkClear->Enable(true);
}

void cMain::uiError()
{
	btnConnect->SetLabel("Connect");

	tlaConnected->SetLabel("Disconnected");
	tlaConnected->SetForegroundColour(txtColour4);

	cbxPort->Enable(true);
	cbxBaud->Enable(true);
	chkReset->Enable(true);
	chkClear->Enable(true);
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