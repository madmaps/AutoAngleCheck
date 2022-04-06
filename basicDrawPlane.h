#ifndef BASICDRAWPLANE_H
#define BASICDRAWPLANE_H
#include <wx/wx.h>
#include <opencv2/opencv.hpp>
#include "imageanalyzer.h"
#include "part.h"

class BasicDrawPlane : public wxPanel
{
public:
    BasicDrawPlane(wxFrame* parent);
    void addPart(Part* inPart);
    void setCameraPhysicalDimensions(const float inPhysicalWidth, const float inPhysicalHeight);
    void startCapture();
    void clear();
    void paintEvent(wxPaintEvent& evt);
    void paintNow();
    void render(wxDC& dc);
    void updateSize(int inSize);
    void moveCursor(wxMouseEvent& evt);
    void setAngleAverage(wxTextCtrl* inAngleAverage);
    void setSampleSize(wxTextCtrl* inSampleSize);


private:
    cv::Mat frame;
    cv::VideoCapture camera;
    bool goodCamera;
    ImageAnalyzer* myImageAnal;
    Part* myPart;
    unsigned int radiusSize;
    unsigned int analyzeSize;
    unsigned int cursorX;
    unsigned int cursorY;
    float imageWidth;
    float imageHeight;
    float physicalWidth;
    float physicalHeight;
    bool captureMode;
    unsigned int savedCursorX;
    unsigned int savedCursorY;
    unsigned int savedRadius;
    unsigned int savedAngleStep;
    float currentStep;
    std::vector<float> capturedAngles;
    wxTextCtrl* angleAverage;
    wxTextCtrl* sampleSize;

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
