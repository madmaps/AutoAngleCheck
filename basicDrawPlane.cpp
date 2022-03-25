#include "basicDrawPlane.h"
#include <wx/wx.h>
#include <iostream>


BEGIN_EVENT_TABLE(BasicDrawPlane, wxPanel)
EVT_PAINT(BasicDrawPlane::paintEvent)
EVT_LEFT_DOWN(BasicDrawPlane::moveCursor)
END_EVENT_TABLE()

using namespace std;

BasicDrawPlane::BasicDrawPlane(wxFrame* parent) : wxPanel(parent)
{
    imageWidth = 640;
    imageHeight = 480;
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
    captureMode = false;
    captureStep = 1;

    if(!camera.isOpened())
    {
       goodCamera = false;
    }
}

void BasicDrawPlane::startCapture()
{
    captureMode = true;
    currentStep = 0;
    capturedAngles.clear();
}

void BasicDrawPlane::setRadialSeal(const bool inIsRadial)
{
    isRadialSeal = inIsRadial;
}

void BasicDrawPlane::setRadialSealProperties(const float inPartMylarRadius, const float inPartYOffset)
{
    partMylarRadius = inPartMylarRadius;
    partYOffset = inPartYOffset;
}

void BasicDrawPlane::setPartAngles(const float inLowYellowAngle, const float inLowGreenAngle, const float inHighGreenAngle, const float inHighYellowAngle)
{
    lowYellowAngle = inLowYellowAngle;
    lowGreenAngle = inLowGreenAngle;
    highGreenAngle = inHighGreenAngle;
    highYellowAngle = inHighYellowAngle;
}

void BasicDrawPlane::setCameraPhysicalDimensions(const float inPhysicalWidth, const float inPhysicalHeight)
{
    physicalWidth = inPhysicalWidth;
    physicalHeight = inPhysicalHeight;
}

void BasicDrawPlane::setStartCapturePoint(const unsigned int inCaptureStartX, const unsigned int inCaptureStartY)
{
    captureStartX = inCaptureStartX;
    captureStartY = inCaptureStartY;
}

void BasicDrawPlane::setEndCapturePoint(const unsigned int inCaptureEndX, const unsigned int inCaptureEndY)
{
    captureEndX = inCaptureEndX;
    captureEndY = inCaptureEndY;
}

void BasicDrawPlane::setCaptureStep(const unsigned int inCaptureStep)
{
    captureStep = inCaptureStep;
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
    if(captureMode)
    {
        float length = vectorLength((int)captureEndX - (int)captureStartX, (int)captureEndY - (int)captureStartY);
        float percentDone = currentStep / length;
        float currentX = (int)captureStartX + ((int)captureEndX - (int)captureStartX) * percentDone;
        float currentY = (int)captureStartY + ((int)captureEndY - (int)captureStartY) * percentDone;
        myImageAnal->setAnalyzLength(16);
        radiusSize = 16;
        myImageAnal->setPiviotPoint(currentX, currentY);
        float goodAngle = 360 * 2 - myImageAnal->getAngle();
        if(isRadialSeal)
        {
            float offset = partMylarRadius  - sqrt(pow(0 - toPhysicalX(cursorX) , 2) + pow(partMylarRadius - toPhysicalY(imageHeight - cursorY), 2));
            float cursorAngle = findDotProdAngle(0, partMylarRadius, 0 - toPhysicalX(cursorX), partMylarRadius - toPhysicalY(imageHeight - cursorY));
            if(cursorX < 320)
            {
                cursorAngle = -cursorAngle;
            }
            goodAngle = toMylarAngle((goodAngle) + cursorAngle, partMylarRadius, offset);
        }
        if(goodAngle > -90 && goodAngle < 90)
        {
            capturedAngles.push_back(goodAngle);
        }

        cursorX = currentX;
        cursorY = currentY;
        currentStep += captureStep;
        if(percentDone > 1)
        {
            captureMode = false;
            float total = 0;
            for(float angle : capturedAngles)
            {
                total += angle;
            }
            cout << total / capturedAngles.size() << endl;
        }
    }
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

    drawAngleRanges(dc);


    drawAngle(dc, myImageAnal->getAngle(), cursorX, cursorY, radiusSize, wxColour(255, 100, 0, 255));

    wxFont newFont;
    newFont.SetPointSize(16);
    dc.SetPen(currentPen);
    dc.SetFont(newFont);
    dc.SetTextForeground(wxColor(255, 100, 0));
    float goodAngle = 360 * 2 - myImageAnal->getAngle();
    if(isRadialSeal)
    {
        float offset = partMylarRadius  - sqrt(pow(0 - toPhysicalX(cursorX) , 2) + pow(partMylarRadius - toPhysicalY(imageHeight - cursorY), 2));
        float cursorAngle = findDotProdAngle(0, partMylarRadius, 0 - toPhysicalX(cursorX), partMylarRadius - toPhysicalY(imageHeight - cursorY));
        if(cursorX < 320)
        {
            cursorAngle = -cursorAngle;
        }
        goodAngle = toMylarAngle((goodAngle) + cursorAngle, partMylarRadius, offset);
    }
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

void BasicDrawPlane::drawAngleRanges(wxDC& inDC)
{
    float cursorAngle = 0;
    float yellowLow = lowYellowAngle;
    float greenLow = lowGreenAngle;
    float greenHigh = highGreenAngle;
    float yellowHigh = highYellowAngle;
    if(isRadialSeal)
    {
        float offset = partMylarRadius  - sqrt(pow(0 - toPhysicalX(cursorX) , 2) + pow(partMylarRadius - toPhysicalY(imageHeight - cursorY), 2));
        cursorAngle = findDotProdAngle(0, partMylarRadius, 0 - toPhysicalX(cursorX), partMylarRadius - toPhysicalY(imageHeight - cursorY));
        if(cursorX < 320)
        {
            cursorAngle = -cursorAngle;
        }
        yellowLow = toCameraAngle(lowYellowAngle, partMylarRadius, offset) - cursorAngle;
        greenLow = toCameraAngle(lowGreenAngle, partMylarRadius, offset) - cursorAngle;
        greenHigh = toCameraAngle(highGreenAngle, partMylarRadius, offset) - cursorAngle;
        yellowHigh = toCameraAngle(highYellowAngle, partMylarRadius, offset) - cursorAngle;
    }
    for(unsigned int i = 0; i < 2; i++)
    {
        drawArc(inDC, greenLow + (180 * i), greenHigh + (180 * i), cursorX, cursorY, radiusSize, wxColour(0, 255, 0, 64));
        drawArc(inDC, yellowLow + (180 * i), greenLow + (180 * i), cursorX, cursorY, radiusSize, wxColour(255, 255, 0, 64));
        drawArc(inDC, greenHigh + (180 * i), yellowHigh + (180 * i), cursorX, cursorY, radiusSize, wxColour(255, 255, 0, 64));
        drawArc(inDC, yellowHigh + (180 * i), yellowLow + (180 * (i + 1)), cursorX, cursorY, radiusSize, wxColour(255, 0, 0, 15));
    }

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

float BasicDrawPlane::dotProduct(const float inPointOneX, const float inPointOneY, const float inPointTwoX, const float inPointTwoY)const
{
    return (inPointOneX * inPointTwoX) + (inPointOneY * inPointTwoY);
}

float BasicDrawPlane::vectorLength(const float inPointX,const float inPointY)const
{
    return sqrt(pow(inPointX, 2) + pow(inPointY, 2));
}

float BasicDrawPlane::findDotProdAngle(const float inPointOneX, const float inPointOneY, const float inPointTwoX, const float inPointTwoY)const
{
    return acos(dotProduct(inPointOneX, inPointOneY, inPointTwoX, inPointTwoY) / (vectorLength(inPointOneX, inPointOneY) * vectorLength(inPointTwoX, inPointTwoY))) * 180 / M_PI;
}

float BasicDrawPlane::toMylarAngle(float inAngle, float mylarRadius, float yOffset)const
{
    if(inAngle > 180)
    {
        inAngle -= 180;
    }
    float correctValue = 0;
    bool negInAngle = false;
    if(inAngle < 0)
    {
        negInAngle = true;
        inAngle = fabs(inAngle);
    }
    if(inAngle !=0 && inAngle != 180 && inAngle != 360)
    {
        float R = 0;
        float decimalPoint = 10;
        correctValue = 180 - inAngle;
        float K = mylarRadius - yOffset;
        while(decimalPoint > .0001)
        {
            R = K * sin(correctValue * M_PI / 180) + K * cos(correctValue * M_PI / 180)
                * tan((180 - (correctValue + inAngle)) * M_PI / 180);
            if(R > mylarRadius)
            {
                correctValue += decimalPoint;
                decimalPoint /= 10;
            }
            else
            {
                correctValue -= decimalPoint;
            }
        }
    }
    if(negInAngle)
    {
        correctValue = -(90 - (180 - (correctValue + inAngle)));
    }
    else
    {
        correctValue = (90 - (180 - (correctValue + inAngle)));
    }
    return correctValue;
}

float BasicDrawPlane::toCameraAngle(float inAngle, float mylarRadius, float yOffset) const
{
    float K = mylarRadius - yOffset;
    float R = 0;
    float correctValue = 0;
    float decimalPoint = 1;
    while(decimalPoint > 0.0001)

    {
        R = sqrt(pow((mylarRadius - correctValue), 2) + pow(correctValue * tan(inAngle * M_PI / 180), 2));
        if(R < K)
        {
            correctValue -= decimalPoint;
            decimalPoint /= 10;
        }
        else
        {
            correctValue += decimalPoint;
        }
    }
    return 180 - (180 - (90 + inAngle) + atan((mylarRadius - correctValue) / (correctValue * tan(inAngle * M_PI / 180))) * 180 / M_PI);
}

float BasicDrawPlane::toPhysicalX(const float inX)const
{
    return -(physicalWidth / 2) + (inX / (float)imageWidth) * physicalWidth;
}

float BasicDrawPlane::toPhysicalY(const float inY)const
{
    return partYOffset + (-(physicalHeight / 2) + (inY / (float)imageHeight) * physicalHeight);
}



