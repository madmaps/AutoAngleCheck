#include "cMain.h"
#include <iostream>
#include <wx/wx.h>
#include "fixture.h"
//#include "fixtureData.h"

using namespace std;


BEGIN_EVENT_TABLE(cMain, wxFrame)
    EVT_CLOSE(cMain::onClose)
    EVT_COMMAND_SCROLL(100001,cMain::sizeEvent)
    EVT_TEXT(100054, cMain::partTextChange)
    EVT_TEXT(100053, cMain::fixTextChange)
    EVT_TEXT(100077, cMain::serialTextChange)
    EVT_BUTTON(100023, cMain::capture)
    EVT_BUTTON(100017, cMain::clear)
    EVT_LISTBOX(100082, cMain::changePartCmd)
    EVT_LISTBOX(100027, cMain::changeFixtureCmd)
END_EVENT_TABLE()

bool cmpTwoParts(Part* partOne, Part* partTwo)
{
    return (*partOne < *partTwo);
}

cMain::cMain() : wxFrame(nullptr, wxID_ANY, "Auto Angle Checker",wxPoint(50, 50), wxSize(1920, 1080))
{
    currentFixtureData = 0;
    Part* newPart = new Part("42J2235", 43, 44, 50, 51, FALSE, 7, 0.5f, 150, 240, 500, 240, 10, 64, 1, 20, 70);
    Fixture* newFixture0 = new Fixture("5428");
    newFixture0->addDataPoint(new FixtureData("SN0025634", 45.323, "", std::chrono::system_clock::now()));
    newFixture0->addDataPoint(new FixtureData("SN0025635", 46.549, "", std::chrono::system_clock::now()));
    newFixture0->addDataPoint(new FixtureData("SN0025636", 41.881, "", std::chrono::system_clock::now()));
    newFixture0->addDataPoint(new FixtureData("SN0025637", 48.725, "", std::chrono::system_clock::now()));
    newFixture0->addDataPoint(new FixtureData("SN0025638", 44.522, "", std::chrono::system_clock::now()));
    newPart->addFixture(newFixture0);
    Fixture* newFixture1 = new Fixture("4332");
    newFixture1->addDataPoint(new FixtureData("SN0024634", 43.323, "", std::chrono::system_clock::now()));
    newFixture1->addDataPoint(new FixtureData("SN0025635", 47.549, "", std::chrono::system_clock::now()));
    newFixture1->addDataPoint(new FixtureData("SN0025666", 43.881, "", std::chrono::system_clock::now()));
    newFixture1->addDataPoint(new FixtureData("SN0025337", 49.725, "", std::chrono::system_clock::now()));
    newFixture1->addDataPoint(new FixtureData("SN0022238", 42.522, "", std::chrono::system_clock::now()));
    newPart->addFixture(newFixture1);


    Part* newPart1 = new Part("36C2154", 42, 45, 50, 52, FALSE, 7, 0.5f, 150, 240, 500, 240, 10, 64, 1, 0, 90);
    Part* newPart2 = new Part("81M2177", 30, 36, 40, 44, FALSE, 4, 0.25f, 150, 240, 500, 240, 10, 64, 1, 0, 360);
    Part* newPart3 = new Part("89J2182", 15, 20, 25, 30, FALSE, 7, 0.5f, 150, 240, 500, 240, 10, 64, 1, 0, 360);
    Part* newPart4 = new Part("47I2019", 43, 44, 50, 51, FALSE, 7, 0.5f, 150, 240, 500, 240, 10, 64, 1, 0, 360);
    Part* newPart5 = new Part("52A3081", 72, 75, 80, 82, FALSE, 7, 0.5f, 150, 240, 500, 240, 10, 64, 1, 0, 360);
    listOfParts.push_back(newPart);
    listOfParts.push_back(newPart1);
    listOfParts.push_back(newPart2);
    listOfParts.push_back(newPart3);
    listOfParts.push_back(newPart4);
    listOfParts.push_back(newPart5);
    sort(listOfParts.begin(),listOfParts.end(),cmpTwoParts);




    this->SetBackgroundColour(wxColour(200, 200, 200));
    wxBoxSizer* leftTopSizer = new wxBoxSizer(wxHORIZONTAL);
    drawPlane = new BasicDrawPlane(this);
    drawPlane->addPart(listOfParts.at(0));
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
    wxButton* clearButton = new wxButton(this, 100017, "CLEAR");
    clearButton->SetMaxSize(wxSize(120, 50));
    clearButton->SetMinSize(wxSize(120, 50));
    buttonSizer->Add(captureButton, 0, wxEXPAND | wxALL | wxCENTER, 10);
    buttonSizer->Add(clearButton, 0, wxEXPAND | wxALL, 10);
    buttonSizer->Add(submitButton, 0, wxEXPAND | wxALL, 10);



    wxStaticText* angleAverageText = new wxStaticText(this, wxID_ANY, "Average Angle:");
    angleAverage = new wxTextCtrl(this, 100045);
    angleAverage->Enable(false);
    drawPlane->setAngleAverage(angleAverage);
    wxStaticText* sampleSizeText = new wxStaticText(this, wxID_ANY, "Samples:");
    wxTextCtrl* sampleSize = new wxTextCtrl(this, 100044);
    sampleSize->Enable(false);
    drawPlane->setSampleSize(sampleSize);
    wxBoxSizer* angleInfo = new wxBoxSizer(wxHORIZONTAL);
    angleInfo->Add(angleAverageText, 0, wxEXPAND | wxALL, 10);
    angleInfo->Add(angleAverage, 0, wxEXPAND | wxALL, 10);
    angleInfo->Add(sampleSizeText, 0, wxEXPAND | wxALL, 10);
    angleInfo->Add(sampleSize, 0, wxEXPAND | wxALL, 10);



    wxBoxSizer* leftSizer = new wxBoxSizer(wxVERTICAL);
    leftSizer->Add(leftTopSizer, 0, wxEXPAND | wxALL, 10);
    leftSizer->Add(angleInfo, 0, wxEXPAND | wxALL, 10);

    wxStaticBoxSizer* fixtureSizer = new wxStaticBoxSizer(wxVERTICAL, this, wxString("Fixture"));
    wxBoxSizer* fixtureEditSizer = new wxBoxSizer(wxHORIZONTAL);
    wxStaticText* fixtureStaticText = new wxStaticText(this, wxID_ANY, "Fixture #");
    fixtureTextCtrl = new wxTextCtrl(this, 100053);
    fixtureEditSizer->Add(fixtureStaticText, 0, wxEXPAND | wxALL, 10);
    fixtureEditSizer->Add(fixtureTextCtrl, 0, wxEXPAND | wxALL, 10);

    fixtureListBox = new wxListBox(this, 100027);
    fixtureListBox->SetMaxSize(wxSize(999,100));

    fixtureSizer->Add(fixtureEditSizer, 0, wxEXPAND | wxALL, 10);
    fixtureSizer->Add(fixtureListBox, 0, wxEXPAND | wxALL, 10);

    wxStaticBoxSizer* partNumSizer = new wxStaticBoxSizer(wxVERTICAL, this, wxString("Part"));
    wxBoxSizer* partNumEditSizer = new wxBoxSizer(wxHORIZONTAL);
    wxStaticText* partNumStaticText = new wxStaticText(this, wxID_ANY, "Part #");
    partNumTextCtrl = new wxTextCtrl(this, 100054);
    partNumEditSizer->Add(partNumStaticText, 0, wxEXPAND | wxALL, 10);
    partNumEditSizer->Add(partNumTextCtrl, 0, wxEXPAND | wxALL, 10);

    partNumListBox = new wxListBox(this, 100082);
    partNumListBox->SetMaxSize(wxSize(999,100));
    unsigned int count = 0;
    for(Part* currentPart : listOfParts)
    {
        partNumListBox->Insert(currentPart->getPartName(), count);
        count++;
    }
    partNumListBox->SetSelection(0);

    partNumSizer->Add(partNumEditSizer, 0, wxEXPAND | wxALL, 10);
    partNumSizer->Add(partNumListBox, 0, wxEXPAND | wxALL, 10);

    wxStaticText* serialNumStaticText = new wxStaticText(this, wxID_ANY, wxString("Serial#      "));
    serialNumTextCtrl = new wxTextCtrl(this, 100077);
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
    chart->setPart(listOfParts.at(0));
    drawPlane->setAngleChart(chart);
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

void cMain::clear(wxCommandEvent& evt)
{
    drawPlane->clear();
}

cMain::~cMain()
{
    delete timer;
    for(Part* deleteMe : listOfParts)
    {
        delete deleteMe;
    }
}

void cMain::partTextChange(wxCommandEvent& evt)
{
    int index = 0;
    unsigned int counter = 1;
    for(Part* searchPart : listOfParts)
    {
        if(searchPart->getPartName().compare(partNumTextCtrl->GetLineText(0).c_str()) < 0)
        {
            index = counter;
        }
        counter++;
    }
    partNumListBox->SetSelection(index);
    changePart();
}

void cMain::fixTextChange(wxCommandEvent& evt)
{
    if(partNumListBox->GetSelection() >= 0)
    {
        int index = 0;
        unsigned int counter = 1;
        std::vector<Fixture*> listOfFixtures = listOfParts.at(partNumListBox->GetSelection())->getFixtureList();
        for(Fixture* searchFixture : listOfFixtures)
        {
            if(searchFixture->getSerialNumber().compare(fixtureTextCtrl->GetLineText(0).c_str()) < 0)
            {
                index = counter;
            }
            counter++;
        }
        fixtureListBox->SetSelection(index);
        changeFixture();
    }
}

void cMain::serialTextChange(wxCommandEvent& evt)
{
    if(fixtureListBox->GetSelection() >= 0)
    {
        if(currentFixtureData == 0)
        {
            currentFixtureData = new FixtureData(std::string(serialNumTextCtrl->GetLineText(0).c_str()), 0, "", std::chrono::system_clock::now());
            Fixture* currentFixture = listOfParts.at(partNumListBox->GetSelection())->getFixtureList().at(fixtureListBox->GetSelection());
            currentFixture->addDataPoint(currentFixtureData, false);
        }
        else
        {
            currentFixtureData->setSerialNumber(std::string(serialNumTextCtrl->GetLineText(0).c_str()));
        }
        chart->Refresh();
        chart->Update();
    }
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

void cMain::changePartCmd(wxCommandEvent& evt)
{
    changePart();
}

void cMain::changeFixtureCmd(wxCommandEvent& evt)
{
    changeFixture();
}


void cMain::changePart()
{
    Part* newPart = listOfParts.at(partNumListBox->GetSelection());
    drawPlane->addPart(newPart);
    chart->setPart(newPart);
    std::vector<Fixture*> listOfFixtures = newPart->getFixtureList();
    fixtureListBox->Clear();
    unsigned int count = 0;
    for(Fixture* currentFixture : listOfFixtures)
    {
        fixtureListBox->Insert(currentFixture->getSerialNumber(), count);
        count++;
    }
    if(listOfFixtures.size() > 0)
    {
        chart->setFixture(0);
        fixtureListBox->SetSelection(0);
    }
}

void cMain::changeFixture()
{
    drawPlane->setCurrentFixture(fixtureListBox->GetSelection());
    chart->setFixture(fixtureListBox->GetSelection());
    chart->Refresh();
    chart->Update();
}

