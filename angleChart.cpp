#include "angleChart.h"
#include <wx/wx.h>
#include <iostream>

BEGIN_EVENT_TABLE(AngleChart, wxPanel)
EVT_PAINT(AngleChart::paintEvent)
END_EVENT_TABLE()


AngleChart::AngleChart(wxFrame* inParent) : wxScrolledWindow(inParent)
{
    this->SetMinSize(wxSize(1260,1080));
    this->SetMaxSize(wxSize(1260, 1080));
    SetVirtualSize(wxSize(1260, 1080));
    SetScrollRate(1,1);
    lowYellowAngle = 42;
    lowGreenAngle = 43;
    highGreenAngle = 50;
    highYellowAngle = 51;
    numCols = 25;
    serialNumberLength = 50;
    colWidth = 20;
    colHeight = 15;
    divisions = 2;
    Show();
}

void AngleChart::paintEvent(wxPaintEvent& evt)
{
    wxPaintDC dc(this);
    render(dc);
}

void AngleChart::render(wxDC& dc)
{
    dc.SetBackground(wxColour(255, 255, 255));
    dc.Clear();
    wxPen currentPen;
    wxBrush currentBrush;
    int redTopRect = serialNumberLength;
    int redHeight = numCols * colHeight;
    //int redHeight = (numCols / 2) * colHeight - ((highYellowAngle - lowYellowAngle) / 2) * divisions * colHeight;
    currentPen.SetColour(wxColour(205, 0, 0));
    currentBrush.SetColour(wxColour(205, 0, 0));
    dc.SetPen(currentPen);
    dc.SetBrush(currentBrush);
    dc.DrawRectangle(0, redTopRect, 1260, redHeight);


    int yellowTopRect = serialNumberLength + (numCols / 2) * colHeight - ((highYellowAngle - lowYellowAngle) / 2) * divisions * colHeight;
    int yellowHeight = (lowGreenAngle - lowYellowAngle) * divisions * colHeight;

    currentPen.SetColour(wxColour(225, 225, 0));
    currentBrush.SetColour(wxColour(225, 225, 0));
    dc.SetPen(currentPen);
    dc.SetBrush(currentBrush);
    dc.DrawRectangle(0, yellowTopRect, 1260, yellowHeight);

    yellowTopRect = serialNumberLength + (numCols / 2) * colHeight + ((highYellowAngle - lowYellowAngle) / 2) * divisions * colHeight;
    yellowHeight = -(highYellowAngle - highGreenAngle) * divisions * colHeight;

    currentPen.SetColour(wxColour(225, 225, 0));
    currentBrush.SetColour(wxColour(225, 225, 0));
    dc.SetPen(currentPen);
    dc.SetBrush(currentBrush);
    dc.DrawRectangle(0, yellowTopRect, 1260, yellowHeight);

    int greenTopRect = serialNumberLength + (numCols / 2) * colHeight - ((highGreenAngle - lowGreenAngle) / 2) * divisions * colHeight;
    int greenHeight = (highGreenAngle - lowGreenAngle) * divisions * colHeight;
    currentPen.SetColour(wxColour(0, 150, 0));
    currentBrush.SetColour(wxColour(0, 150, 0));
    dc.SetPen(currentPen);
    dc.SetBrush(currentBrush);
    dc.DrawRectangle(0, greenTopRect, 1260, greenHeight);


    currentPen.SetColour(wxColour(0, 0, 0));
    dc.SetPen(currentPen);
    for(unsigned int i = 0; i <= numCols; i++)
    {
        dc.DrawLine(0, serialNumberLength + (i * colHeight), 1260, serialNumberLength + (i * colHeight));
    }
    for(unsigned int i = 0; i <= 61; i++)
    {
        dc.DrawLine(40 + (i * colWidth), 0, 40 + (i * colWidth), 1080);
    }




}

wxCoord AngleChart::OnGetRowHeight(size_t row)const
{
    return wxCoord(row);
}

wxCoord AngleChart::OnGetColumnWidth(size_t column)const
{
    return wxCoord(column);
}

