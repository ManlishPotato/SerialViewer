#include "cApp.h"

wxIMPLEMENT_APP(cApp);

cApp::cApp()
{

}

cApp::~cApp()
{

}

bool cApp::OnInit()
{	
	m_frame1=new cMain();
	m_frame1->SetBackgroundColour(wxColour(33,37,43));
	m_frame1->Show();	

	return true;
}
