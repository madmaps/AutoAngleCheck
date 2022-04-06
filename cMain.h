#ifndef CMAIN_H
#define CMAIN_H
#include<wx/wx.h>
#include <opencv2/opencv.hpp>
#include "imageanalyzer.h"
#include "renderTimer.h"
#include "basicDrawPlane.h"
#include "angleChart.h"




class cMain : public wxFrame
{
public:
    cMain();
    ~cMain();
    void onClose(wxCloseEvent& evt);
    void sizeEvent(wxScrollEvent& evt);
    void capture(wxCommandEvent& evt);
    void clear(wxCommandEvent& evt);
    wxTextCtrl* angleAverage;


private:
    RenderTimer* timer;
    BasicDrawPlane* drawPlane;
    AngleChart* chart;

    DECLARE_EVENT_TABLE()
};

#endif // CMAIN_H
