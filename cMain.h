#pragma once
#include "wx/wx.h"

class cMain : public wxFrame
{
	public:
		cMain();
		~cMain();
		
	public:
		wxTextCtrl* txt1=nullptr;
		wxTextCtrl* txt2=nullptr;
		wxButton* btn1=nullptr;
		wxComboBox* box1=nullptr;
		wxComboBox* box2=nullptr;
		wxComboBox* box3=nullptr;
		wxCheckBox* check1=nullptr;
		wxCheckBox* check2=nullptr;
		
		wxMenuBar* menubar;
		wxMenu* file;
};

