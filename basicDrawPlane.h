#ifndef BASICDRAWPLANE_H
#define BASICDRAWPLANE_H
#include <wx/wx.h>
#include <opencv2/opencv.hpp>
#include "imageanalyzer.h"


class BasicDrawPlane : public wxPanel
{
public:
    BasicDrawPlane(wxFrame* parent);

    void paintEvent(wxPaintEvent& evt);
    void paintNow();
    void render(wxDC& dc);

private:
    cv::Mat frame;
    cv::VideoCapture camera;
    bool goodCamera;
    ImageAnalyzer* myImageAnal;

    void drawArc(wxDC& inDC, float startAngle, float endAngle,int inLocX, int inLocY, int inRad, wxColour inColor);
    void drawAngle(wxDC& inDC, float inAngle, int inLocX, int inLocY, int inRad, wxColour inColor);

    DECLARE_EVENT_TABLE()
};

#endif // BASICDRAWPLANE_H
