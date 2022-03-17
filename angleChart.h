#ifndef ANGLECHART_H
#define ANGLECHART_H
#include <wx/wx.h>
#include <wx/vscroll.h>

class AngleChart : public wxScrolledWindow
{
public:
    AngleChart(wxFrame* inParent);
    void paintEvent(wxPaintEvent& evt);
    void render(wxDC& dc);
    virtual wxCoord OnGetRowHeight(size_t row)const;
    virtual wxCoord OnGetColumnWidth(size_t column)const;
private:
    float lowYellowAngle;
    float lowGreenAngle;
    float highGreenAngle;
    float highYellowAngle;
    unsigned int numCols;
    unsigned int serialNumberLength;
    unsigned int colWidth;
    unsigned int colHeight;
    unsigned int divisions;
    DECLARE_EVENT_TABLE()
};

#endif // ANGLECHART_H
