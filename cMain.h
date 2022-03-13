#ifndef CMAIN_H
#define CMAIN_H
#include<wx/wx.h>
#include <opencv2/opencv.hpp>
#include "imageanalyzer.h"
#include "renderTimer.h"
#include "basicDrawPlane.h"




class cMain : public wxFrame
{
public:
    cMain();
    ~cMain();
    void onClose(wxCloseEvent& evt);
    void sizeEvent(wxScrollEvent& evt);

private:
    RenderTimer* timer;
    BasicDrawPlane* drawPlane;
    DECLARE_EVENT_TABLE()
};

#endif // CMAIN_H
