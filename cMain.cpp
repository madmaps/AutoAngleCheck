#include "cMain.h"
#include <iostream>
#include <wx/wx.h>

using namespace std;


BEGIN_EVENT_TABLE(cMain, wxFrame)
    EVT_CLOSE(cMain::onClose)
    EVT_COMMAND_SCROLL(100001,cMain::sizeEvent)
    EVT_BUTTON(100023, cMain::capture)
END_EVENT_TABLE()


cMain::cMain() : wxFrame(nullptr, wxID_ANY, "Auto Angle Checker",wxPoint(50, 50), wxSize(1920, 1080))
{
    this->SetBackgroundColour(wxColour(200, 200, 200));
    wxBoxSizer* leftTopSizer = new wxBoxSizer(wxHORIZONTAL);
    drawPlane = new BasicDrawPlane(this);
    drawPlane->setPartAngles(10, 15, 25, 30);
    drawPlane->setRadialSeal(true);
    drawPlane->setRadialSealProperties(4, 1.5);
    drawPlane->setCameraPhysicalDimensions(1, 0.5);
    drawPlane->setStartCapturePoint(100, 240);
    drawPlane->setEndCapturePoint(540, 240);
    drawPlane->setCaptureStep(10);
    leftTopSizer->Add(drawPlane, 0, wxEXPAND | wxALL, 10);
    wxSlider* sizeSlider = new wxSlider(this, 100001, 64, 8, 128, wxDefaultPosition, wxDefaultSize, wxSL_VERTICAL);
    leftTopSizer->Add(sizeSlider, 0, wxEXPAND | wxALL, 10);


    wxButton* captureButton = new wxButton(this, 100023, "CAPTURE");
    captureButton->SetMaxSize(wxSize(120, 50));
    captureButton->SetMinSize(wxSize(120, 50));
    wxButton* submitButton = new wxButton(this, wxID_ANY, "SUBMIT");
    submitButton->SetMaxSize(wxSize(120, 50));
    submitButton->SetMinSize(wxSize(120, 50));
    wxBoxSizer* buttonSizer = new wxBoxSizer(wxHORIZONTAL);
    buttonSizer->Add(captureButton, 0, wxEXPAND | wxALL | wxCENTER, 10);
    buttonSizer->Add(submitButton, 0, wxEXPAND | wxALL, 10);

    wxBoxSizer* leftSizer = new wxBoxSizer(wxVERTICAL);
    leftSizer->Add(leftTopSizer, 0, wxEXPAND | wxALL, 10);

    //leftSizer->Add(buttonSizer, 0, wxEXPAND | wxALL, 10);

    wxStaticBoxSizer* fixtureSizer = new wxStaticBoxSizer(wxVERTICAL, this, wxString("Fixture"));
    wxBoxSizer* fixtureEditSizer = new wxBoxSizer(wxHORIZONTAL);
    wxStaticText* fixtureStaticText = new wxStaticText(this, wxID_ANY, "Fixture #");
    wxTextCtrl* fixtureTextCtrl = new wxTextCtrl(this, wxID_ANY);
    fixtureEditSizer->Add(fixtureStaticText, 0, wxEXPAND | wxALL, 10);
    fixtureEditSizer->Add(fixtureTextCtrl, 0, wxEXPAND | wxALL, 10);

    wxListBox* fixtureListBox = new wxListBox(this, wxID_ANY);
    fixtureListBox->SetMaxSize(wxSize(999,100));
    fixtureListBox->Insert(wxString("5142"), 0);
    fixtureListBox->Insert(wxString("2141"), 0);
    fixtureListBox->Insert(wxString("3169"), 0);
    fixtureListBox->Insert(wxString("2219"), 0);
    fixtureListBox->Insert(wxString("9814"), 0);


    fixtureSizer->Add(fixtureEditSizer, 0, wxEXPAND | wxALL, 10);
    fixtureSizer->Add(fixtureListBox, 0, wxEXPAND | wxALL, 10);

    wxStaticBoxSizer* partNumSizer = new wxStaticBoxSizer(wxVERTICAL, this, wxString("Part"));
    wxBoxSizer* partNumEditSizer = new wxBoxSizer(wxHORIZONTAL);
    wxStaticText* partNumStaticText = new wxStaticText(this, wxID_ANY, "Part #");
    wxTextCtrl* partNumTextCtrl = new wxTextCtrl(this, wxID_ANY);
    partNumEditSizer->Add(partNumStaticText, 0, wxEXPAND | wxALL, 10);
    partNumEditSizer->Add(partNumTextCtrl, 0, wxEXPAND | wxALL, 10);

    wxListBox* partNumListBox = new wxListBox(this, wxID_ANY);
    partNumListBox->SetMaxSize(wxSize(999,100));
    partNumListBox->Insert(wxString("50J196"), 0);
    partNumListBox->Insert(wxString("53K425"), 0);
    partNumListBox->Insert(wxString("53K426"), 0);
    partNumListBox->Insert(wxString("53K427"), 0);
    partNumListBox->Insert(wxString("53K428"), 0);
    partNumListBox->Insert(wxString("4326050"), 0);
    partNumListBox->Insert(wxString("4111614"), 0);


    partNumSizer->Add(partNumEditSizer, 0, wxEXPAND | wxALL, 10);
    partNumSizer->Add(partNumListBox, 0, wxEXPAND | wxALL, 10);

    wxStaticText* serialNumStaticText = new wxStaticText(this, wxID_ANY, wxString("Serial#      "));
    wxTextCtrl* serialNumTextCtrl = new wxTextCtrl(this, wxID_ANY);
    serialNumTextCtrl->SetMinSize(wxSize(150,40));
    wxBoxSizer* serialNumSizer = new wxBoxSizer(wxHORIZONTAL);
    serialNumSizer->Add(serialNumStaticText, 0, wxEXPAND | wxALL, 10);
    serialNumSizer->Add(serialNumTextCtrl, 0, wxEXPAND | wxALL, 10);

    wxBoxSizer* topRightSizer = new wxBoxSizer(wxHORIZONTAL);
    topRightSizer->Add(partNumSizer, 0, wxEXPAND | wxALL, 10);
    topRightSizer->Add(fixtureSizer, 0, wxEXPAND | wxALL, 10);

    wxStaticText* commentStaticText = new wxStaticText(this, wxID_ANY, wxString("Comment"));
    wxTextCtrl* commentTextCtrl = new wxTextCtrl(this, wxID_ANY);
    commentTextCtrl->SetMinSize(wxSize(300, 40));
    wxBoxSizer* commentSizer = new wxBoxSizer(wxHORIZONTAL);
    commentSizer->Add(commentStaticText, 0, wxEXPAND | wxALL, 10);
    commentSizer->Add(commentTextCtrl, 0, wxEXPAND | wxALL, 10);



    wxStaticBoxSizer* serialCommentSizer = new wxStaticBoxSizer(wxVERTICAL, this, "Information");
    serialCommentSizer->Add(serialNumSizer, 0, wxEXPAND | wxALL, 10);
    serialCommentSizer->Add(commentSizer, 0, wxEXPAND | wxALL, 10);

    wxBoxSizer* rightSizer = new wxBoxSizer(wxVERTICAL);
    rightSizer->Add(topRightSizer, 0, wxEXPAND | wxALL, 10);
    rightSizer->Add(serialCommentSizer, 0, wxEXPAND | wxALL, 10);
    rightSizer->Add(buttonSizer, 0 , wxEXPAND | wxALL, 10);



    wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
    sizer->Add(leftSizer, 0, wxEXPAND | wxALL, 10);
    sizer->Add(rightSizer, 0, wxEXPAND | wxALL, 10);
    wxBoxSizer* topSizer = new wxBoxSizer(wxVERTICAL);
    topSizer->Add(sizer, 0, wxEXPAND | wxALL, 10);
    chart = new AngleChart(this);

    topSizer->Add(chart, 0, wxEXPAND | wxALL, 10);


    SetSizer(topSizer);

    timer = new RenderTimer(drawPlane);
    Show();
    timer->start();

}

void cMain::capture(wxCommandEvent& evt)
{
    drawPlane->startCapture();
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

