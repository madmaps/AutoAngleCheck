#ifndef CMAIN_H
#define CMAIN_H
#include<wx/wx.h>
#include <opencv2/opencv.hpp>
#include "imageanalyzer.h"
#include "renderTimer.h"
#include "basicDrawPlane.h"
#include "angleChart.h"
#include "fixtureData.h"




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
    void fixTextChange(wxCommandEvent& evt);
    void changePartCmd(wxCommandEvent& evt);
    void changeFixtureCmd(wxCommandEvent& evt);
    void serialTextChange(wxCommandEvent& evt);


private:
    void changePart();
    void changeFixture();
    RenderTimer* timer;
    BasicDrawPlane* drawPlane;
    AngleChart* chart;
    std::vector<Part*> listOfParts;
    wxListBox* partNumListBox;
    wxListBox* fixtureListBox;
    wxTextCtrl* partNumTextCtrl;
    wxTextCtrl* fixtureTextCtrl;
    wxTextCtrl* angleAverage;
    wxTextCtrl* serialNumTextCtrl;
    FixtureData* currentFixtureData;


    DECLARE_EVENT_TABLE()
};

#endif // CMAIN_H
