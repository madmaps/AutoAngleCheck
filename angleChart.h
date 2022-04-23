#ifndef ANGLECHART_H
#define ANGLECHART_H
#include <wx/wx.h>
#include <wx/vscroll.h>
#include "part.h"

class AngleChart : public wxScrolledWindow
{
public:
    AngleChart(wxFrame* inParent);
    void paintEvent(wxPaintEvent& evt);
    void render(wxDC& dc);
    void setPart(Part* inPart);
    void setFixture(const unsigned int inFixture);
    Fixture* getCurrentFixture()const;
    void clearUnsubmitted();
    void submit();
private:
    Part* currentPart;
    int currentFixture;
    unsigned int width;
    unsigned int height;
    unsigned int numCols;
    unsigned int numRows;
    unsigned int serialNumberLength;
    unsigned int angleTextLength;
    unsigned int rowWidth;
    unsigned int colHeight;
    unsigned int divisions;
    DECLARE_EVENT_TABLE()
};

#endif // ANGLECHART_H
