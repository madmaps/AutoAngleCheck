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
    void partTextChange(wxCommandEvent& evt);
    void changePartCmd(wxCommandEvent& evt);


private:
    void changePart();
    RenderTimer* timer;
    BasicDrawPlane* drawPlane;
    AngleChart* chart;
    std::vector<Part*> listOfParts;
    wxListBox* partNumListBox;
    wxTextCtrl* partNumTextCtrl;
    wxTextCtrl* angleAverage;


    DECLARE_EVENT_TABLE()
};

#endif // CMAIN_H
