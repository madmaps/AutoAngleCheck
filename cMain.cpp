#include "cMain.h"
#include <iostream>
#include <wx/wx.h>

using namespace std;


BEGIN_EVENT_TABLE(cMain, wxFrame)
EVT_CLOSE(cMain::onClose)
END_EVENT_TABLE()


cMain::cMain() : wxFrame(nullptr, wxID_ANY, "Auto Angle Checker",wxPoint(50, 50), wxSize(640, 480))
{
    wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
    drawPlane = new BasicDrawPlane(this);
    sizer->Add(drawPlane, 1, wxEXPAND);
    SetSizer(sizer);

    timer = new RenderTimer(drawPlane);
    Show();
    timer->start();

}

cMain::~cMain()
{
    delete timer;
}

void cMain::onClose(wxCloseEvent& evt)
{
    timer->Stop();
    evt.Skip();
}


