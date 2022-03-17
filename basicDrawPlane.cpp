#include "basicDrawPlane.h"
#include <wx/wx.h>


BEGIN_EVENT_TABLE(BasicDrawPlane, wxPanel)
EVT_PAINT(BasicDrawPlane::paintEvent)
EVT_LEFT_DOWN(BasicDrawPlane::moveCursor)
END_EVENT_TABLE()


BasicDrawPlane::BasicDrawPlane(wxFrame* parent) : wxPanel(parent)
{
    this->SetMinSize(wxSize(640,480));
    myImageAnal = new ImageAnalyzer();
    myImageAnal->setAngleRange(360 , 360 * 2);
    //myImageAnal->setAngleRange(0, 360);
    goodCamera = true;
    camera.open(0);
    camera.set(cv::CAP_PROP_FRAME_WIDTH , 640);
    camera.set(cv::CAP_PROP_FRAME_HEIGHT, 480);
    radiusSize = 64;
    cursorX = 640 / 2;
    cursorY = 480 / 2;

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

void BasicDrawPlane::updateSize(int inSize)
{
    myImageAnal->setAnalyzLength(inSize);
    radiusSize = inSize;
    paintNow();
}

void BasicDrawPlane::moveCursor(wxMouseEvent& evt)
{
    wxPoint currentPoint = evt.GetPosition();
    if((int)currentPoint.x + (int)radiusSize < 640 && (int)currentPoint.x - (int)radiusSize > 0 && (int)currentPoint.y + (int)radiusSize < 480 && (int)currentPoint.y - (int)radiusSize > 0)
    {
        cursorX = currentPoint.x;
        cursorY = currentPoint.y;
        paintNow();
    }
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
    myImageAnal->setPiviotPoint(cursorX, cursorY);
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
    dc.DrawCircle(cursorX, cursorY, radiusSize);
    dc.DrawLine(cursorX - radiusSize / 3 , cursorY, cursorX + radiusSize / 3 , cursorY);
    dc.DrawLine(cursorX, cursorY - radiusSize / 3, cursorX, cursorY + radiusSize / 3);


    drawArc(dc, 44, 60, cursorX, cursorY, radiusSize, wxColour(0, 255, 0, 64));
    drawArc(dc, 40, 44, cursorX, cursorY, radiusSize, wxColour(255, 255, 0, 64));
    drawArc(dc, 60, 64, cursorX, cursorY, radiusSize, wxColour(255, 255, 0, 64));
    drawArc(dc, 64, 40, cursorX, cursorY, radiusSize, wxColour(255, 255, 255, 64));

    drawAngle(dc, myImageAnal->getAngle(), cursorX, cursorY, radiusSize, wxColour(255, 100, 0, 255));
    wxFont newFont;
    newFont.SetPointSize(16);
    dc.SetPen(currentPen);
    dc.SetFont(newFont);
    dc.SetTextForeground(wxColor(255, 100, 0));
    float goodAngle = 360 * 2 - myImageAnal->getAngle();
    dc.DrawText(wxString::FromDouble(goodAngle, 2), cursorX - 25, cursorY + 20);

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
    currentPen.SetWidth(2);
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

