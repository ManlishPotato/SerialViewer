#include "cMain.h"

int baseId=10000;
int btn1Id=baseId++;
int check1Id=baseId++;
int check2Id=baseId++;
int check3Id=baseId++;
int settingsId=baseId++;
int aboutId=baseId++;


const int box1ChoiceNum=2;
const int box2ChoiceNum=2;
const int box3ChoiceNum=2;
const wxString box1Choice[box1ChoiceNum]={"9600","115200"};
const wxString box2Choice[box2ChoiceNum]={"COM3","COM4"};
const wxString box3Choice[box3ChoiceNum]={"Delim: /n/l","Delim: >"};

cMain::cMain() : wxFrame(nullptr,wxID_ANY,"SerialViewer1.0",wxPoint(30,30),wxSize(650,600))
{
	txt1=new wxTextCtrl(this,wxID_ANY,"Transceive",wxPoint(10,10),wxSize(500,28));
	txt2=new wxTextCtrl(this,wxID_ANY,"Receive",wxPoint(10,50),wxSize(500,450));
	btn1=new wxButton(this,btn1Id,"Connect",wxPoint(520,10),wxSize(100,50));
	box1=new wxComboBox(this,wxID_ANY,"9600",wxPoint(520,90),wxSize(100,25),box1ChoiceNum,box1Choice);
	box2=new wxComboBox(this,wxID_ANY,"COM4",wxPoint(520,140),wxSize(100,25),box2ChoiceNum,box2Choice);
	box3=new wxComboBox(this,wxID_ANY,"Delim: /n/l",wxPoint(120,500),wxSize(100,25),box3ChoiceNum,box3Choice);
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