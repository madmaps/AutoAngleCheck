#ifndef BASICDRAWPLANE_H
#define BASICDRAWPLANE_H
#include <wx/wx.h>
#include <opencv2/opencv.hpp>
#include "imageanalyzer.h"


class BasicDrawPlane : public wxPanel
{
public:
    BasicDrawPlane(wxFrame* parent);
    void setPartAngles(const float inLowYellowAngle, const float inLowGreenAngle, const float inHighGreenAngle, const float inHighYellowAngle);
    void setCameraPhysicalDimensions(const float inPhysicalWidth, const float inPhysicalHeight);
    void setRadialSeal(const bool inIsRadial);
    void setRadialSealProperties(const float inPartMylarRadius, const float inPartYOffset);
    void setStartCapturePoint(const unsigned int inCaptureStartX, const unsigned int inCaptureStartY);
    void setEndCapturePoint(const unsigned int inCaptureEndX, const unsigned int inCaptureEndY);
    void setCaptureStep(const unsigned int inCaptureStep);
    void paintEvent(wxPaintEvent& evt);
    void paintNow();
    void render(wxDC& dc);
    void updateSize(int inSize);
    void moveCursor(wxMouseEvent& evt);


private:
    cv::Mat frame;
    cv::VideoCapture camera;
    bool goodCamera;
    ImageAnalyzer* myImageAnal;
    unsigned int radiusSize;
    unsigned int cursorX;
    unsigned int cursorY;
    float lowYellowAngle;
    float lowGreenAngle;
    float highGreenAngle;
    float highYellowAngle;
    float imageWidth;
    float imageHeight;
    float physicalWidth;
    float physicalHeight;
    bool isRadialSeal;
    float partMylarRadius;
    float partYOffset;
    bool captureMode;
    unsigned int captureStartX;
    unsigned int captureStartY;
    unsigned int captureEndX;
    unsigned int captureEndY;
    unsigned int captureStep;

    void drawArc(wxDC& inDC, float startAngle, float endAngle,int inLocX, int inLocY, int inRad, wxColour inColor);
    void drawAngle(wxDC& inDC, float inAngle, int inLocX, int inLocY, int inRad, wxColour inColor);
    void drawAngleRanges(wxDC& inDC);

    float toMylarAngle(float inAngle, float mylarRadius, float yOffset)const;
    float toCameraAngle(float inAngle, float mylarRaduis, float yOffset)const;

    float dotProduct(const float inPointOneX, const float inPointOneY, const float inPointTwoX, const float inPointTwoY)const;
    float vectorLength(const float inPointX,const float inPointY)const;
    float findDotProdAngle(const float inPointOneX, const float inPointOneY, const float inPointTwoX, const float inPointTwoY)const;
    float toPhysicalX(const float inX)const;
    float toPhysicalY(const float inY)const;


    DECLARE_EVENT_TABLE()
};

#endif // BASICDRAWPLANE_H
