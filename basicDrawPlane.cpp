#include "basicDrawPlane.h"
#include <wx/wx.h>
#include <iostream>
#include <wx/dcbuffer.h>


BEGIN_EVENT_TABLE(BasicDrawPlane, wxPanel)
EVT_PAINT(BasicDrawPlane::paintEvent)
EVT_LEFT_DOWN(BasicDrawPlane::moveCursor)
END_EVENT_TABLE()

using namespace std;

BasicDrawPlane::BasicDrawPlane(wxFrame* parent) : wxPanel(parent)
{
    imageWidth = 640;
    imageHeight = 480;
    this->SetMinSize(wxSize(imageWidth,imageHeight));
    myImageAnal = new ImageAnalyzer();
    myImageAnal->setAngleRange(0, 359);
    goodCamera = true;
    camera.open(0);
    camera.set(cv::CAP_PROP_FRAME_WIDTH , imageWidth);
    camera.set(cv::CAP_PROP_FRAME_HEIGHT, imageHeight);
    radiusSize = 128;
    analyzeSize = 64;
    cursorX = imageWidth / 2;
    cursorY = imageHeight / 2;
    captureMode = false;
    currentFixture = -1;
    myPart = 0;
    if(!camera.isOpened())
    {
       goodCamera = false;
    }
}

void BasicDrawPlane::startCapture()
{
    savedCursorX = cursorX;
    savedCursorY = cursorY;
    savedRadius = analyzeSize;
    savedAngleStep = myImageAnal->getAngleStep();
    savedStartAngle = myImageAnal->getStartAngle();
    savedEndAngle = myImageAnal->getEndAngle();
    captureMode = true;
    currentStep = 0;
}

void BasicDrawPlane::clear()
{
    capturedAngles.clear();
    angleAverage->ChangeValue(wxString(""));
    sampleSize->ChangeValue(wxString(""));
}

void BasicDrawPlane::addPart(Part* inPart)
{
    myPart = inPart;
}

void BasicDrawPlane::setAngleAverage(wxTextCtrl* inAngleAverage)
{
    angleAverage = inAngleAverage;
}

void BasicDrawPlane::setSampleSize(wxTextCtrl* inSampleSize)
{
    sampleSize = inSampleSize;
}

void BasicDrawPlane::setAngleChart(AngleChart* inAngleChart)
{
    chart = inAngleChart;
}


void BasicDrawPlane::setCameraPhysicalDimensions(const float inPhysicalWidth, const float inPhysicalHeight)
{
    physicalWidth = inPhysicalWidth;
    physicalHeight = inPhysicalHeight;
}

void BasicDrawPlane::paintEvent(wxPaintEvent &evt)
{
    wxBufferedPaintDC dc(this);
    this->PrepareDC(dc);
    render(dc);
}

void BasicDrawPlane::updateSize(int inSize)
{
    myImageAnal->setAnalyzLength(inSize);
    analyzeSize = inSize;
    paintNow();
}

void BasicDrawPlane::setCurrentFixture(const int inCurrentFixture)
{
    currentFixture = inCurrentFixture;
}

void BasicDrawPlane::setSubmitButton(wxButton* inButton)
{
    submitButton = inButton;
}


void BasicDrawPlane::moveCursor(wxMouseEvent& evt)
{
    wxPoint currentPoint = evt.GetPosition();
    if((int)currentPoint.x + (int)analyzeSize < 640 && (int)currentPoint.x - (int)analyzeSize > 0 && (int)currentPoint.y + (int)analyzeSize < 480 && (int)currentPoint.y - (int)analyzeSize > 0)
    {
        cursorX = currentPoint.x;
        cursorY = currentPoint.y;
        paintNow();
    }
}


void BasicDrawPlane::paintNow()
{
    wxBufferedPaintDC dc(this);
    render(dc);
}

void BasicDrawPlane::render(wxDC& dc)
{
    dc.SetBackground(*wxWHITE_BRUSH);
    dc.Clear();
    camera >> frame;
    wxPen currentPen;
    wxBrush currentBrush;
    myImageAnal->setImageData(frame.data,frame.cols,frame.rows);
    myImageAnal->setPiviotPoint(cursorX, cursorY);
    wxImage bitMap(frame.cols, frame.rows, frame.data, true);
    dc.DrawBitmap(wxBitmap(bitMap, 24), 0, 0);
    float origonalAngleCapture = 0;
    if(captureMode && myPart != 0)
    {
        currentPen.SetWidth(25);
        currentPen.SetColour(wxColour(255, 255 , 255, 100));
        dc.SetPen(currentPen);
        dc.DrawLine(myPart->getStartCaptureX(), myPart->getStartCaptureY(), myPart->getEndCaptureX(), myPart->getEndCaptureY());

        float length = vectorLength((int)myPart->getEndCaptureX() - (int)myPart->getStartCaptureX(), (int)myPart->getEndCaptureY() - (int)myPart->getStartCaptureY());
        float percentDone = currentStep / length;
        float currentX = (int)myPart->getStartCaptureX() + ((int)myPart->getEndCaptureX() - (int)myPart->getStartCaptureX()) * percentDone;
        float currentY = (int)myPart->getStartCaptureY() + ((int)myPart->getEndCaptureY() - (int)myPart->getStartCaptureY()) * percentDone;
        myImageAnal->setAnalyzLength(myPart->getCaptureRadius());
        myImageAnal->setAngleStep(myPart->getCaptureAngleStep());
        myImageAnal->setAngleRange(myPart->getCaptureStartAngle(), myPart->getCaptureEndAngle());
        analyzeSize = myPart->getCaptureRadius();
        myImageAnal->setPiviotPoint(currentX, currentY);
        //float goodAngle = myImageAnal->getAngle();
        origonalAngleCapture = myImageAnal->getAngle();
        float goodAngle = origonalAngleCapture;//myImageAnal->getAngleNonDCT();

        if(goodAngle > 180)
        {
            goodAngle -= 180;
        }
        if(myPart->getIsRadialSeal())
        {
            float offset = myPart->getMylarRadius() - sqrt(pow(0 - toPhysicalX(cursorX) , 2) + pow(myPart->getMylarRadius() - toPhysicalY(imageHeight - cursorY), 2));
            float cursorAngle = findDotProdAngle(0, myPart->getMylarRadius(), 0 - toPhysicalX(cursorX), myPart->getMylarRadius() - toPhysicalY(imageHeight - cursorY));
            if(cursorX < (imageWidth / 2))
            {
                cursorAngle = -cursorAngle;
            }
            goodAngle = toMylarAngle((goodAngle) - cursorAngle, myPart->getMylarRadius(), offset);
        }
        capturedAngles.push_back(goodAngle);
        cursorX = currentX;
        cursorY = currentY;
        currentStep += myPart->getCaptureStep();
        if(percentDone > 1)
        {
            cursorX = savedCursorX;
            cursorY = savedCursorY;
            analyzeSize = savedRadius;
            myImageAnal->setAnalyzLength(savedRadius);
            myImageAnal->setAngleStep(savedAngleStep);
            myImageAnal->setAngleRange(savedStartAngle, savedEndAngle);
            captureMode = false;
            float total = 0;
            for(float angle : capturedAngles)
            {
                total += angle;
            }
            averageAngle = total / capturedAngles.size();
            if(currentFixture != -1)
            {
                Fixture* thisFixture = myPart->getFixtureList().at(currentFixture);
                if(!thisFixture->getIsSubmitted())
                {
                    FixtureData* currentData = thisFixture->getFixtureData(myPart->getFixtureList().at(currentFixture)->getDataSize()-1);
                    currentData->setAngleValue(averageAngle);
                    chart->Refresh();
                    chart->Update();
                }
            }
            angleAverage->ChangeValue(wxString::FromDouble(averageAngle, 2));
            sampleSize->ChangeValue(wxString::FromDouble((double)capturedAngles.size(),0));
            if(capturedAngles.size() > 140)
            {
                if(averageAngle >= myPart->getLowGreenAngle() && averageAngle <= myPart->getHighGreenAngle())
                {
                    submitButton->SetBackgroundColour(wxColor(0,255,0));
                    submitButton->Enable(true);
                }
                else if(averageAngle >= myPart->getLowYellowAngle() && averageAngle <= myPart->getHighYellowAngle())
                {
                    submitButton->SetBackgroundColour(wxColor(255, 255, 0));
                    submitButton->Enable(true);
                }
                else
                {
                    submitButton->SetBackgroundColour(wxColour(255, 0, 0));
                    submitButton->Enable(true);
                }
            }
        }
    }
    else
    {
        origonalAngleCapture = myImageAnal->getAngle();
    }
    currentPen.SetWidth(1);
    currentPen.SetColour(wxColour(0, 0 , 0));
    currentBrush.SetColour(wxColour(0, 255, 0, 64));
    currentBrush.SetStyle(wxBRUSHSTYLE_TRANSPARENT);
    dc.SetPen(currentPen);
    dc.SetBrush(currentBrush);
    dc.DrawCircle(cursorX, cursorY, radiusSize);
    dc.DrawLine(cursorX - radiusSize / 3 , cursorY, cursorX + radiusSize / 3 , cursorY);
    dc.DrawLine(cursorX, cursorY - radiusSize / 3, cursorX, cursorY + radiusSize / 3);

    drawAngleRanges(dc);


    //drawAngle(dc, myImageAnal->getAngle(), cursorX, cursorY, radiusSize * 1.2, wxColour(255, 100, 0, 255));
    drawAngle(dc, origonalAngleCapture, cursorX, cursorY, radiusSize * 1.2, wxColour(255, 100, 0, 255));

    currentPen.SetWidth(2);
    currentPen.SetColour(wxColour(128, 128, 128));
    currentPen.SetStyle(wxPENSTYLE_SHORT_DASH);
    currentBrush.SetStyle(wxBRUSHSTYLE_TRANSPARENT);
    dc.SetPen(currentPen);
    dc.SetBrush(currentBrush);
    dc.DrawCircle(cursorX, cursorY, analyzeSize);


    wxFont newFont;
    newFont.SetPointSize(16);
    dc.SetPen(currentPen);
    dc.SetFont(newFont);
    dc.SetTextForeground(wxColor(255, 100, 0));
    //float goodAngle = 360 * 2 - myImageAnal->getAngle();
    //float goodAngle = myImageAnal->getAngle();
    float goodAngle = origonalAngleCapture;
    if(myPart != 0)
    {
        if(myPart->getIsRadialSeal())
        {
            float offset = myPart->getMylarRadius()  - sqrt(pow(0 - toPhysicalX(cursorX) , 2) + pow(myPart->getMylarRadius() - toPhysicalY(imageHeight - cursorY), 2));
            float cursorAngle = findDotProdAngle(0, myPart->getMylarRadius(), 0 - toPhysicalX(cursorX), myPart->getMylarRadius() - toPhysicalY(imageHeight - cursorY));
            if(cursorX < (imageWidth / 2))
            {
                cursorAngle = -cursorAngle;
            }
            goodAngle = toMylarAngle((goodAngle) + cursorAngle, myPart->getMylarRadius(), offset);
        }
    }
    if(goodAngle > 180)
    {
        goodAngle -= 180;
    }
    dc.DrawText(wxString::FromDouble(goodAngle, 2), cursorX - 25, cursorY + 20);
    if(myPart != 0 && capturedAngles.size() > 0 && !captureMode)
    {
        wxColour angleColor(255, 0, 0, 255);
        if(averageAngle <= myPart->getHighYellowAngle() && averageAngle >= myPart->getLowYellowAngle())
        {
            angleColor.Set(255, 255, 0, 255);
        }
        if(averageAngle <= myPart->getHighGreenAngle() && averageAngle >= myPart->getLowGreenAngle())
        {
            angleColor.Set(0, 255, 0, 255);
        }
        goodAngle = averageAngle;
        if(myPart->getIsRadialSeal())
        {
            float offset = myPart->getMylarRadius()  - sqrt(pow(0 - toPhysicalX(cursorX) , 2) + pow(myPart->getMylarRadius() - toPhysicalY(imageHeight - cursorY), 2));
            float cursorAngle = findDotProdAngle(0, myPart->getMylarRadius(), 0 - toPhysicalX(cursorX), myPart->getMylarRadius() - toPhysicalY(imageHeight - cursorY));
            if(cursorX < (imageWidth / 2))
            {
                cursorAngle = -cursorAngle;
            }
            goodAngle = toCameraAngle(averageAngle + cursorAngle, myPart->getMylarRadius(), offset);
        }

        drawAngle(dc, goodAngle, cursorX, cursorY, radiusSize, angleColor);
    }
}
void BasicDrawPlane::drawAngle(wxDC& inDC, float inAngle, int inLocX, int inLocY, int inRad, wxColour inColor)
{
    float xStartRad = ((inAngle) * M_PI) / 180;
    float xStart = inRad * sin(xStartRad);
    float xEnd = xStart + inLocX;
    xStart = inLocX - xStart;

    float yStartRad = ((inAngle) * M_PI) / 180;
    float yStart = inRad * cos(yStartRad);
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
    if(myPart != 0)
    {
        float cursorAngle = 0;
        float yellowLow = myPart->getLowYellowAngle();
        float greenLow = myPart->getLowGreenAngle();
        float greenHigh = myPart->getHighGreenAngle();
        float yellowHigh = myPart->getHighYellowAngle();
        if(myPart->getIsRadialSeal())
        {
            float offset = myPart->getMylarRadius() - sqrt(pow(0 - toPhysicalX(cursorX) , 2) + pow(myPart->getMylarRadius() - toPhysicalY(imageHeight - cursorY), 2));
            cursorAngle = findDotProdAngle(0, myPart->getMylarRadius(), 0 - toPhysicalX(cursorX), myPart->getMylarRadius() - toPhysicalY(imageHeight - cursorY));
            if(cursorX < 320)
            {
                cursorAngle = -cursorAngle;
            }
            yellowLow = toCameraAngle(myPart->getLowYellowAngle(), myPart->getMylarRadius(), offset) - cursorAngle;
            greenLow = toCameraAngle(myPart->getLowGreenAngle(), myPart->getMylarRadius(), offset) - cursorAngle;
            greenHigh = toCameraAngle(myPart->getHighGreenAngle(), myPart->getMylarRadius(), offset) - cursorAngle;
            yellowHigh = toCameraAngle(myPart->getHighYellowAngle(), myPart->getMylarRadius(), offset) - cursorAngle;
        }
        for(unsigned int i = 0; i < 2; i++)
        {
            drawArc(inDC, greenLow + (180 * i), greenHigh + (180 * i), cursorX, cursorY, radiusSize, wxColour(0, 255, 0, 64));
            drawArc(inDC, yellowLow + (180 * i), greenLow + (180 * i), cursorX, cursorY, radiusSize, wxColour(255, 255, 0, 64));
            drawArc(inDC, greenHigh + (180 * i), yellowHigh + (180 * i), cursorX, cursorY, radiusSize, wxColour(255, 255, 0, 64));
            drawArc(inDC, yellowHigh + (180 * i), yellowLow + (180 * (i + 1)), cursorX, cursorY, radiusSize, wxColour(255, 0, 0, 32));
        }
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
    if(inAngle > 90)
    {
        negInAngle = true;
        inAngle -= 90;
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
        correctValue = 90 + (90 - (180 - (correctValue + inAngle)));
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
    return myPart->getYOffset() + (-(physicalHeight / 2) + (inY / (float)imageHeight) * physicalHeight);
}



