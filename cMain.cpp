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
    EVT_BUTTON(100014, cMain::submit)
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
    Part* newPart = new Part("42J2235", 43, 44, 50, 51, true, 2.25f, 0.25f, 150, 240, 500, 240, 10, 64, 1, 0, 359, 140);
    newPart->addFixture(new Fixture("5428"));    newPart->addFixture(new Fixture("4332"));
    newPart->addFixture(new Fixture("5144"));
    /*newFixture0->addDataPoint(new FixtureData("SN0025634", 45.323, "", std::chrono::system_clock::now()));
    newFixture0->addDataPoint(new FixtureData("SN0025635", 46.549, "", std::chrono::system_clock::now()));
    newFixture0->addDataPoint(new FixtureData("SN0025636", 41.881, "", std::chrono::system_clock::now()));
    newFixture0->addDataPoint(new FixtureData("SN0025637", 48.725, "", std::chrono::system_clock::now()));
    newFixture0->addDataPoint(new FixtureData("SN0025638", 44.522, "", std::chrono::system_clock::now()));*/
    /*newFixture1->addDataPoint(new FixtureData("SN0024634", 43.323, "", std::chrono::system_clock::now()));
    newFixture1->addDataPoint(new FixtureData("SN0025635", 47.549, "", std::chrono::system_clock::now()));
    newFixture1->addDataPoint(new FixtureData("SN0025666", 43.881, "", std::chrono::system_clock::now()));
    newFixture1->addDataPoint(new FixtureData("SN0025337", 49.725, "", std::chrono::system_clock::now()));
    newFixture1->addDataPoint(new FixtureData("SN0022238", 42.522, "", std::chrono::system_clock::now()));*/

    Part* newPart1 = new Part("36C2154", 43, 44, 51, 52, false, 2.25, 0.25f, 150, 240, 500, 240, 10, 64, 1, 0, 359, 140);
    newPart1->addFixture(new Fixture("4819"));
    newPart1->addFixture(new Fixture("2217"));
    newPart1->addFixture(new Fixture("5418"));
    newPart1->addFixture(new Fixture("1222"));

    Part* newPart2 = new Part("81M2177", 30, 36, 40, 44, false, 4, 0.25f, 150, 240, 500, 240, 10, 64, 1, 0, 359, 140);
    newPart2->addFixture(new Fixture("4472"));
    newPart2->addFixture(new Fixture("6819"));
    newPart2->addFixture(new Fixture("4112"));
    newPart2->addFixture(new Fixture("9631"));
    newPart2->addFixture(new Fixture("2866"));


    Part* newPart3 = new Part("89J2182", 15, 20, 25, 30, false, 7, 0.5f, 150, 240, 500, 240, 10, 64, 1, 0, 359, 140);
    newPart3->addFixture(new Fixture("1171"));
    newPart3->addFixture(new Fixture("9987"));

    Part* newPart4 = new Part("47I2019", 43, 44, 50, 51, false, 7, 0.5f, 150, 240, 500, 240, 10, 64, 1, 0, 359, 140);
    newPart4->addFixture(new Fixture("7319"));
    newPart4->addFixture(new Fixture("3217"));
    newPart4->addFixture(new Fixture("2870"));
    newPart4->addFixture(new Fixture("7800"));
    newPart4->addFixture(new Fixture("2468"));
    newPart4->addFixture(new Fixture("9112"));
    newPart4->addFixture(new Fixture("4376"));
    newPart4->addFixture(new Fixture("0012"));

    Part* newPart5 = new Part("52A3081", 72, 75, 80, 82, false, 7, 0.5f, 150, 240, 500, 240, 10, 64, 1, 0, 359, 140);
    newPart5->addFixture(new Fixture("3113"));
    newPart5->addFixture(new Fixture("6629"));
    newPart5->addFixture(new Fixture("8714"));


    listOfParts.push_back(newPart);
    listOfParts.push_back(newPart1);
    listOfParts.push_back(newPart2);
    listOfParts.push_back(newPart3);
    listOfParts.push_back(newPart4);
    listOfParts.push_back(newPart5);
    sort(listOfParts.begin(),listOfParts.end(),cmpTwoParts);

    widgetPadding = 4;

    this->SetBackgroundColour(wxColour(200, 200, 200));
    wxBoxSizer* leftTopSizer = new wxBoxSizer(wxHORIZONTAL);
    drawPlane = new BasicDrawPlane(this);
    if(listOfParts.size() > 0)
    {
        drawPlane->addPart(listOfParts.at(0));
    }
    leftTopSizer->Add(drawPlane, 0, wxEXPAND | wxALL, widgetPadding);
    wxSlider* sizeSlider = new wxSlider(this, 100001, 64, 8, 128, wxDefaultPosition, wxDefaultSize, wxSL_VERTICAL);
    leftTopSizer->Add(sizeSlider, 0, wxEXPAND | wxALL, widgetPadding);


    wxButton* captureButton = new wxButton(this, 100023, "CAPTURE");
    captureButton->SetMaxSize(wxSize(120, 50));
    captureButton->SetMinSize(wxSize(120, 50));
    submitButton = new wxButton(this, 100014, "SUBMIT");
    submitButton->Enable(false);
    submitButton->SetMaxSize(wxSize(120, 50));
    submitButton->SetMinSize(wxSize(120, 50));
    wxBoxSizer* buttonSizer = new wxBoxSizer(wxHORIZONTAL);
    wxButton* clearButton = new wxButton(this, 100017, "CLEAR");
    clearButton->SetMaxSize(wxSize(120, 50));
    clearButton->SetMinSize(wxSize(120, 50));
    buttonSizer->Add(captureButton, 0, wxEXPAND | wxALL | wxCENTER, widgetPadding);
    buttonSizer->Add(clearButton, 0, wxEXPAND | wxALL, widgetPadding);
    buttonSizer->Add(submitButton, 0, wxEXPAND | wxALL, widgetPadding);
    drawPlane->setSubmitButton(submitButton);



    wxStaticText* angleAverageText = new wxStaticText(this, wxID_ANY, "Average Angle:");
    angleAverage = new wxTextCtrl(this, 100045);
    angleAverage->Enable(false);
    drawPlane->setAngleAverage(angleAverage);
    wxStaticText* sampleSizeText = new wxStaticText(this, wxID_ANY, "Samples:");
    wxTextCtrl* sampleSize = new wxTextCtrl(this, 100044);
    sampleSize->Enable(false);
    drawPlane->setSampleSize(sampleSize);
    wxBoxSizer* angleInfo = new wxBoxSizer(wxHORIZONTAL);
    angleInfo->Add(angleAverageText, 0, wxEXPAND | wxALL, widgetPadding);
    angleInfo->Add(angleAverage, 0, wxEXPAND | wxALL, widgetPadding);
    angleInfo->Add(sampleSizeText, 0, wxEXPAND | wxALL, widgetPadding);
    angleInfo->Add(sampleSize, 0, wxEXPAND | wxALL, widgetPadding);



    wxBoxSizer* leftSizer = new wxBoxSizer(wxVERTICAL);
    leftSizer->Add(leftTopSizer, 0, wxEXPAND | wxALL, widgetPadding);
    leftSizer->Add(angleInfo, 0, wxEXPAND | wxALL, widgetPadding);

    wxStaticBoxSizer* fixtureSizer = new wxStaticBoxSizer(wxVERTICAL, this, wxString("Fixture"));
    wxBoxSizer* fixtureEditSizer = new wxBoxSizer(wxHORIZONTAL);
    wxStaticText* fixtureStaticText = new wxStaticText(this, wxID_ANY, "Fixture #");
    fixtureTextCtrl = new wxTextCtrl(this, 100053);
    fixtureEditSizer->Add(fixtureStaticText, 0, wxEXPAND | wxALL, widgetPadding);
    fixtureEditSizer->Add(fixtureTextCtrl, 0, wxEXPAND | wxALL, widgetPadding);

    fixtureListBox = new wxListBox(this, 100027);
    fixtureListBox->SetMaxSize(wxSize(999,100));

    fixtureSizer->Add(fixtureEditSizer, 0, wxEXPAND | wxALL, widgetPadding);
    fixtureSizer->Add(fixtureListBox, 0, wxEXPAND | wxALL, widgetPadding);

    wxStaticBoxSizer* partNumSizer = new wxStaticBoxSizer(wxVERTICAL, this, wxString("Part"));
    wxBoxSizer* partNumEditSizer = new wxBoxSizer(wxHORIZONTAL);
    wxStaticText* partNumStaticText = new wxStaticText(this, wxID_ANY, "Part #");
    partNumTextCtrl = new wxTextCtrl(this, 100054);
    partNumEditSizer->Add(partNumStaticText, 0, wxEXPAND | wxALL, widgetPadding);
    partNumEditSizer->Add(partNumTextCtrl, 0, wxEXPAND | wxALL, widgetPadding);

    partNumListBox = new wxListBox(this, 100082);
    partNumListBox->SetMaxSize(wxSize(999,100));
    unsigned int count = 0;
    for(Part* currentPart : listOfParts)
    {
        partNumListBox->Insert(currentPart->getPartName(), count);
        count++;
    }
    if(listOfParts.size() > 0)
    {
        partNumListBox->SetSelection(0);
    }

    partNumSizer->Add(partNumEditSizer, 0, wxEXPAND | wxALL, widgetPadding);
    partNumSizer->Add(partNumListBox, 0, wxEXPAND | wxALL, widgetPadding);

    wxStaticText* serialNumStaticText = new wxStaticText(this, wxID_ANY, wxString("Serial#      "));
    serialNumTextCtrl = new wxTextCtrl(this, 100077);
    serialNumTextCtrl->SetMinSize(wxSize(150,40));
    wxBoxSizer* serialNumSizer = new wxBoxSizer(wxHORIZONTAL);
    serialNumSizer->Add(serialNumStaticText, 0, wxEXPAND | wxALL, widgetPadding);
    serialNumSizer->Add(serialNumTextCtrl, 0, wxEXPAND | wxALL, widgetPadding);

    wxBoxSizer* topRightSizer = new wxBoxSizer(wxHORIZONTAL);
    topRightSizer->Add(partNumSizer, 0, wxEXPAND | wxALL, widgetPadding);
    topRightSizer->Add(fixtureSizer, 0, wxEXPAND | wxALL, widgetPadding);

    wxStaticText* commentStaticText = new wxStaticText(this, wxID_ANY, wxString("Comment"));
    wxTextCtrl* commentTextCtrl = new wxTextCtrl(this, wxID_ANY);
    commentTextCtrl->SetMinSize(wxSize(300, 40));
    wxBoxSizer* commentSizer = new wxBoxSizer(wxHORIZONTAL);
    commentSizer->Add(commentStaticText, 0, wxEXPAND | wxALL, widgetPadding);
    commentSizer->Add(commentTextCtrl, 0, wxEXPAND | wxALL, widgetPadding);



    wxStaticBoxSizer* serialCommentSizer = new wxStaticBoxSizer(wxVERTICAL, this, "Information");
    serialCommentSizer->Add(serialNumSizer, 0, wxEXPAND | wxALL, widgetPadding);
    serialCommentSizer->Add(commentSizer, 0, wxEXPAND | wxALL, widgetPadding);

    wxBoxSizer* rightSizer = new wxBoxSizer(wxVERTICAL);
    rightSizer->Add(topRightSizer, 0, wxEXPAND | wxALL, widgetPadding);
    rightSizer->Add(serialCommentSizer, 0, wxEXPAND | wxALL, widgetPadding);
    rightSizer->Add(buttonSizer, 0 , wxEXPAND | wxALL, widgetPadding);



    wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
    sizer->Add(leftSizer, 0, wxEXPAND | wxALL, widgetPadding);
    sizer->Add(rightSizer, 0, wxEXPAND | wxALL, widgetPadding);
    wxBoxSizer* topSizer = new wxBoxSizer(wxVERTICAL);
    topSizer->Add(sizer, 0, wxEXPAND | wxALL, widgetPadding);
    chart = new AngleChart(this);
    if(listOfParts.size() > 0)
    {
        chart->setPart(listOfParts.at(0));
        changePart();
    }
    drawPlane->setAngleChart(chart);
    topSizer->Add(chart, 0, wxEXPAND | wxALL, widgetPadding);


    SetSizer(topSizer);

    timer = new RenderTimer(drawPlane);
    Show();
    timer->start();
    drawPlane->setCameraPhysicalDimensions(0.386f, 0.327f);

}


void cMain::capture(wxCommandEvent& evt)
{
    drawPlane->startCapture();
}

void cMain::clear(wxCommandEvent& evt)
{
    drawPlane->clear();
    submitButton->Enable(false);
    submitButton->SetBackgroundColour(wxColor(128, 128, 128));
}

cMain::~cMain()
{
    delete timer;
    for(Part* deleteMe : listOfParts)
    {
        delete deleteMe;
    }
}

void cMain::submit(wxCommandEvent& evt)
{
    chart->submit();
    chart->Refresh();
    chart->Update();
    drawPlane->clear();
    currentFixtureData = 0;
    serialNumTextCtrl->ChangeValue("");
    submitButton->Enable(false);
    submitButton->SetBackgroundColour(wxColor(128, 128, 128));
}


void cMain::partTextChange(wxCommandEvent& evt)
{
    int index = -1;
    unsigned int counter = 1;
    for(Part* searchPart : listOfParts)
    {
        if(searchPart->getPartName().compare(partNumTextCtrl->GetLineText(0).c_str()) < 0)
        {
            index = counter;
        }
        counter++;
    }
    if(index >= 0)
    {
        partNumListBox->SetSelection(index);
    }
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
    if(listOfParts.size() > 0)
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
        changeFixture();
    }
}

void cMain::changeFixture()
{
    chart->clearUnsubmitted();
    drawPlane->clear();
    currentFixtureData = 0;
    serialNumTextCtrl->ChangeValue("");
    submitButton->Enable(false);
    submitButton->SetBackgroundColour(wxColor(128, 128, 128));
    if(fixtureListBox->GetCount() > 0)
    {
        drawPlane->setCurrentFixture(fixtureListBox->GetSelection());
        chart->setFixture(fixtureListBox->GetSelection());
    }
    else
    {
        drawPlane->setCurrentFixture(-1);
        chart->setFixture(-1);
    }
    chart->Refresh();
    chart->Update();
}

