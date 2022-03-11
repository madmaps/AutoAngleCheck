#include "basicDrawPlane.h"
#include <wx/wx.h>


BEGIN_EVENT_TABLE(BasicDrawPlane, wxPanel)
EVT_PAINT(BasicDrawPlane::paintEvent)
END_EVENT_TABLE()


BasicDrawPlane::BasicDrawPlane(wxFrame* parent) : wxPanel(parent)
{
    myImageAnal = new ImageAnalyzer();
    myImageAnal->setAngleRange(270 + 360 , 90 + 360*2);
    goodCamera = true;
    camera.open(0);
    camera.set(cv::CAP_PROP_FRAME_WIDTH , 640);
    camera.set(cv::CAP_PROP_FRAME_HEIGHT, 480);

    if(!camera.isOpened())
    {
       goodCamera = false;
    }
}

void BasicDrawPlane::paintEvent(wxPaintEvent &evt)
{
    wxPaintDC dc(this);
    render(dc);
}

void BasicDrawPlane::paintNow()
{
    wxClientDC dc(this);
    render(dc);
}

void BasicDrawPlane::render(wxDC& dc)
{
    dc.SetBackground(*wxWHITE_BRUSH);
    dc.Clear();
    camera >> frame;

    myImageAnal->setImageData(frame.data,frame.cols,frame.rows);
    myImageAnal->setPiviotPoint(frame.cols / 2, frame.rows / 2);
    wxImage test(frame.cols, frame.rows, frame.data, true);
    dc.DrawBitmap(wxBitmap(test, 24), 0, 0);
    wxPen currentPen;
    currentPen.SetWidth(1);
    currentPen.SetColour(wxColour(0, 0 , 0));
    wxBrush currentBrush;
    currentBrush.SetColour(wxColour(0, 255, 0, 64));
    currentBrush.SetStyle(wxBRUSHSTYLE_TRANSPARENT);
    dc.SetPen(currentPen);
    dc.SetBrush(currentBrush);
    dc.DrawCircle(frame.cols / 2, frame.rows / 2, 64);
    dc.DrawLine(frame.cols / 2 - 20 , frame.rows / 2, frame.cols / 2 + 20 , frame.rows / 2);
    dc.DrawLine(frame.cols / 2, frame.rows / 2 - 20, frame.cols / 2, frame.rows / 2 + 20);


    drawArc(dc, 44, 60, frame.cols / 2, frame.rows / 2, 64, wxColour(0, 255, 0, 64));
    drawArc(dc, 40, 44, frame.cols / 2, frame.rows / 2, 64, wxColour(255, 255, 0, 64));
    drawArc(dc, 60, 64, frame.cols / 2, frame.rows / 2, 64, wxColour(255, 255, 0, 64));
    drawArc(dc, 64, 40, frame.cols / 2, frame.rows / 2, 64, wxColour(255, 255, 255, 64));

    drawAngle(dc, myImageAnal->getAngle(),frame.cols / 2, frame.rows / 2, 64, wxColour(0, 0, 0, 255));

}
void BasicDrawPlane::drawAngle(wxDC& inDC, float inAngle, int inLocX, int inLocY, int inRad, wxColour inColor)
{
    float xStartRad = ((180 - inAngle) * 3.1415926535 * 2) / 360;
    float xStart = inRad * 2 * sin(xStartRad);
    float xEnd = xStart + inLocX;
    xStart = inLocX - xStart;

    float yStartRad = ((180 - inAngle) * 3.1415926535 * 2) / 360;
    float yStart = inRad * 2 * cos(yStartRad);
    float yEnd =  inLocY - yStart;
    yStart += inLocY;





    wxPen currentPen;
    currentPen.SetColour(inColor);
    inDC.SetPen(currentPen);
    inDC.DrawLine(xEnd, yEnd, xStart, yStart);
}


void BasicDrawPlane::drawArc(wxDC& inDC, float startAngle, float endAngle,int inLocX, int inLocY, int inRad, wxColour inColor)
{
    float xStartRad = ((180 - startAngle) * 3.1415926535 * 2) / 360;
    float xStart = inRad * sin(xStartRad);
    xStart += inLocX;

    float yStartRad = ((180 - startAngle) * 3.1415926535 * 2) / 360;
    float yStart = inRad * cos(yStartRad);
    yStart += inLocY;

    float xEndRad = ((180 - endAngle) * 3.1415926535 * 2) / 360;
    float xEnd = inRad * sin(xEndRad);
    xEnd += inLocX;

    float yEndRad = ((180 - endAngle) * 3.1415926535 * 2) / 360;
    float yEnd = inRad * cos(yEndRad);
    yEnd += inLocY;
    wxPen currentPen;
    wxBrush currentBrush;
    currentPen.SetColour(inColor);
    currentBrush.SetStyle(wxBRUSHSTYLE_SOLID);
    currentBrush.SetColour(inColor);
    inDC.SetBrush(currentBrush);
    inDC.SetPen(currentPen);
    inDC.DrawArc(xEnd, yEnd, xStart, yStart, inLocX, inLocY);

}

