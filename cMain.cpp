#include "cMain.h"
#include <iostream>
#include <wx/wx.h>

using namespace std;


BEGIN_EVENT_TABLE(cMain, wxFrame)
EVT_CLOSE(cMain::onClose)
EVT_COMMAND_SCROLL(100001,cMain::sizeEvent)
END_EVENT_TABLE()


cMain::cMain() : wxFrame(nullptr, wxID_ANY, "Auto Angle Checker",wxPoint(50, 50), wxSize(1920, 1080))
{
    wxStaticBoxSizer* fixtureSizer = new wxStaticBoxSizer(wxVERTICAL, this, wxString("Fixture"));
    wxBoxSizer* fixtureEditSizer = new wxBoxSizer(wxHORIZONTAL);
    wxStaticText* fixtureStaticText = new wxStaticText(this, wxID_ANY, "Fixture #");
    wxTextCtrl* fixtureTextCtrl = new wxTextCtrl(this, wxID_ANY);
    fixtureEditSizer->Add(fixtureStaticText, 0, wxEXPAND | wxALL, 10);
    fixtureEditSizer->Add(fixtureTextCtrl, 0, wxEXPAND | wxALL, 10);

    wxListBox* fixtureListBox = new wxListBox(this, wxID_ANY);

    fixtureSizer->Add(fixtureEditSizer, 0, wxEXPAND | wxALL, 10);
    fixtureSizer->Add(fixtureListBox, 0, wxEXPAND | wxALL, 10);

    wxStaticBoxSizer* partNumSizer = new wxStaticBoxSizer(wxVERTICAL, this, wxString("Part"));
    wxBoxSizer* partNumEditSizer = new wxBoxSizer(wxHORIZONTAL);
    wxStaticText* partNumStaticText = new wxStaticText(this, wxID_ANY, "Part #");
    wxTextCtrl* partNumTextCtrl = new wxTextCtrl(this, wxID_ANY);
    partNumEditSizer->Add(partNumStaticText, 0, wxEXPAND | wxALL, 10);
    partNumEditSizer->Add(partNumTextCtrl, 0, wxEXPAND | wxALL, 10);

    wxListBox* partNumListBox = new wxListBox(this, wxID_ANY);

    partNumSizer->Add(partNumEditSizer, 0, wxEXPAND | wxALL, 10);
    partNumSizer->Add(partNumListBox, 0, wxEXPAND | wxALL, 10);

    wxBoxSizer* topHorSizer = new wxBoxSizer(wxHORIZONTAL);
    drawPlane = new BasicDrawPlane(this);
    topHorSizer->Add(drawPlane, 0, wxEXPAND | wxALL, 10);
    wxSlider* sizeSlider = new wxSlider(this, 100001, 64, 8, 128, wxDefaultPosition, wxDefaultSize, wxSL_VERTICAL);
    topHorSizer->Add(sizeSlider, 0, wxEXPAND | wxALL, 10);
    topHorSizer->Add(partNumSizer, 0, wxEXPAND | wxALL, 10);
    topHorSizer->Add(fixtureSizer, 0, wxEXPAND | wxALL, 10);


    wxStaticText* serialNumStaticText = new wxStaticText(this, wxID_ANY, wxString("Serial#"));
    wxTextCtrl* serialNumTextCtrl = new wxTextCtrl(this, wxID_ANY);
    wxBoxSizer* serialNumSizer = new wxBoxSizer(wxHORIZONTAL);
    serialNumSizer->Add(serialNumStaticText, 0, wxEXPAND | wxALL, 10);
    serialNumSizer->Add(serialNumTextCtrl, 0, wxEXPAND | wxALL, 10);

    wxStaticText* commentStaticText = new wxStaticText(this, wxID_ANY, wxString("Comment"));
    wxTextCtrl* commentTextCtrl = new wxTextCtrl(this, wxID_ANY);
    wxBoxSizer* commentSizer = new wxBoxSizer(wxHORIZONTAL);
    commentSizer->Add(commentStaticText, 0, wxEXPAND | wxALL, 10);
    commentSizer->Add(commentTextCtrl, 0, wxEXPAND | wxALL, 10);

    wxBoxSizer* serialCommentSizer = new wxBoxSizer(wxVERTICAL);
    serialCommentSizer->Add(serialNumSizer, 0, wxEXPAND | wxALL, 10);
    serialCommentSizer->Add(commentSizer, 0, wxEXPAND | wxALL, 10);

    wxButton* captureButton = new wxButton(this, wxID_ANY, "CAPTURE");
    wxButton* submitButton = new wxButton(this, wxID_ANY, "SUBMIT");
    wxBoxSizer* buttonSizer = new wxBoxSizer(wxVERTICAL);
    buttonSizer->Add(captureButton, 0, wxEXPAND | wxALL, 10);
    buttonSizer->Add(submitButton, 0, wxEXPAND | wxALL, 10);

    wxBoxSizer* middleHorSizer = new wxBoxSizer(wxHORIZONTAL);
    middleHorSizer->Add(buttonSizer, 1, wxEXPAND | wxALL, 10);
    middleHorSizer->Add(serialCommentSizer, 1, wxEXPAND | wxALL, 10);


    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
    sizer->Add(topHorSizer, 0, wxEXPAND | wxALL, 10);
    sizer->Add(middleHorSizer, 0, wxEXPAND | wxALL, 10);

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


