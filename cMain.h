#ifndef CMAIN_H
#define CMAIN_H
#include<wx/wx.h>
#include <opencv2/opencv.hpp>
#include "imageanalyzer.h"


class cMain : public wxFrame
{
public:
    cMain();
    ~cMain();
private:
    cv::Mat frame;
    cv::VideoCapture camera;
    bool goodCamera;
    ImageAnalyzer* myImageAnal;

};

#endif // CMAIN_H
