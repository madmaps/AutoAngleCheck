#include "angleChart.h"
#include <wx/wx.h>
#include <iostream>
#include "fixture.h"

using namespace std;

BEGIN_EVENT_TABLE(AngleChart, wxPanel)
EVT_PAINT(AngleChart::paintEvent)
END_EVENT_TABLE()


AngleChart::AngleChart(wxFrame* inParent) : wxScrolledWindow(inParent, wxID_ANY)
{
    this->SetMinSize(wxSize(1260,400));
    this->SetMaxSize(wxSize(1260, 400));
    SetScrollRate(1,10);
    angleTextLength = 50;
    numCols = 25;
    numRows = 50;
    serialNumberLength = 100;
    rowWidth = 25;
    colHeight = 20;
    divisions = 2;


    width = angleTextLength + (rowWidth * numRows);
    height = serialNumberLength + (colHeight * numCols);

    SetVirtualSize(wxSize(width, height));
}

void AngleChart::paintEvent(wxPaintEvent& evt)
{
    wxPaintDC dc(this);
    this->DoPrepareDC(dc);
    render(dc);
}

void AngleChart::setPart(Part* inPart)
{
    currentPart = inPart;
    currentFixture = -1;
    Refresh();
    Update();
}

void AngleChart::setFixture(const unsigned int inFixture)
{
    currentFixture = inFixture;
    Refresh();
    Update();
}

Fixture* AngleChart::getCurrentFixture() const
{
    if(currentFixture >= 0)
    {
        return currentPart->getFixtureList().at(currentFixture);
    }
    return 0;
}


void AngleChart::render(wxDC& dc)
{
    dc.SetBackground(wxColour(255, 255, 255));
    dc.Clear();
    wxPen currentPen;
    wxBrush currentBrush;
    int redTopRect = serialNumberLength;
    int redHeight = numCols * colHeight;
    currentPen.SetColour(wxColour(205, 0, 0));
    currentBrush.SetColour(wxColour(205, 0, 0));
    dc.SetPen(currentPen);
    dc.SetBrush(currentBrush);
    dc.DrawRectangle(0, redTopRect, width, redHeight);


    int yellowTopRect = serialNumberLength + (numCols / 2) * colHeight - ((currentPart->getHighYellowAngle() - currentPart->getLowYellowAngle()) / 2) * divisions * colHeight;
    int yellowHeight = (currentPart->getHighYellowAngle() - currentPart->getLowYellowAngle()) * divisions * colHeight;

    currentPen.SetColour(wxColour(225, 225, 0));
    currentBrush.SetColour(wxColour(225, 225, 0));
    dc.SetPen(currentPen);
    dc.SetBrush(currentBrush);
    dc.DrawRectangle(0, yellowTopRect, width, yellowHeight);

    int greenTopRect = serialNumberLength + (numCols / 2) * colHeight - ((currentPart->getHighGreenAngle() - currentPart->getLowGreenAngle()) / 2) * divisions * colHeight;
    int greenHeight = (currentPart->getHighGreenAngle() - currentPart->getLowGreenAngle()) * divisions * colHeight;
    currentPen.SetColour(wxColour(0, 150, 0));
    currentBrush.SetColour(wxColour(0, 150, 0));
    dc.SetPen(currentPen);
    dc.SetBrush(currentBrush);
    dc.DrawRectangle(0, greenTopRect, width, greenHeight);

    currentPen.SetColour(wxColour(255, 255, 255));
    currentBrush.SetColour(wxColour(255, 255, 255));
    dc.SetPen(currentPen);
    dc.SetBrush(currentBrush);
    dc.DrawRectangle(0, 0, width, serialNumberLength);

    currentPen.SetColour(wxColour(0, 0, 0));
    dc.SetPen(currentPen);
    for(unsigned int i = 0; i <= numCols; i++)
    {
        dc.DrawLine(0, serialNumberLength + (i * colHeight), width, serialNumberLength + (i * colHeight));
    }
    for(unsigned int i = 0; i <= numRows; i++)
    {
        dc.DrawLine(angleTextLength + (i * rowWidth), 0, angleTextLength + (i * rowWidth), height);
    }
    float greenPixelEnd = serialNumberLength + (numCols / 2) * colHeight + ((currentPart->getHighGreenAngle() - currentPart->getLowGreenAngle()) / 2) * divisions * colHeight;
    float numOfColsToBegin = (greenPixelEnd - serialNumberLength) / colHeight;
    float startingAngle = currentPart->getHighGreenAngle() - (numOfColsToBegin / divisions);
    for(unsigned int i = 0; i <= numCols; i++)
    {
        dc.DrawText(wxString::Format(wxT("%.1f"), (startingAngle + ((float)i / (float)divisions))), 10, serialNumberLength + (i * colHeight));
    }
    if(currentFixture >= 0)
    {
        unsigned int sizeOfData = currentPart->getFixtureList().at(currentFixture)->getDataSize();
        if(sizeOfData > 0)
        {
            FixtureData* currentData = 0;
            for(unsigned int i = 0; i < sizeOfData; i++)
            {
                currentData = currentPart->getFixtureList().at(currentFixture)->getFixtureData(i);
                dc.SetTextForeground(wxColor(0, 0, 0));
                if(i == sizeOfData - 1 && !currentPart->getFixtureList().at(currentFixture)->getIsSubmitted())
                {
                    dc.SetTextForeground(wxColor(255, 0, 0));
                }
                dc.DrawRotatedText(wxString(currentData->getSerialNumber()), angleTextLength + (i * rowWidth), serialNumberLength - 10, 90);
                dc.DrawCircle(angleTextLength + (rowWidth / 2) + (i * rowWidth), serialNumberLength + ((currentData->getAngleValue() - startingAngle) * (colHeight * divisions)), 3);
            }
        }
    }

}

