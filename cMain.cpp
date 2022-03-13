#include "cMain.h"
#include <iostream>
#include <wx/wx.h>

using namespace std;


BEGIN_EVENT_TABLE(cMain, wxFrame)
EVT_CLOSE(cMain::onClose)
EVT_COMMAND_SCROLL(100001,cMain::sizeEvent)
END_EVENT_TABLE()


cMain::cMain() : wxFrame(nullptr, wxID_ANY, "Auto Angle Checker",wxPoint(50, 50), wxSize(700, 480))
{
    wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
    drawPlane = new BasicDrawPlane(this);
    sizer->Add(drawPlane, 1, wxEXPAND);
    wxSlider* sizeSlider = new wxSlider(this, 100001, 64, 8, 128, wxDefaultPosition, wxDefaultSize, wxSL_VERTICAL);
    sizer->Add(sizeSlider, 0, wxEXPAND);

    SetSizer(sizer);

    timer = new RenderTimer(drawPlane);
    Show();
    timer->start();



}

cMain::~cMain()
{
    delete timer;
}

void cMain::sizeEvent(wxScrollEvent &evt)
{
    int newPosition = evt.GetPosition();
    drawPlane->updateSize(newPosition);
}

void cMain::onClose(wxCloseEvent& evt)
{
    timer->Stop();
    evt.Skip();
}


