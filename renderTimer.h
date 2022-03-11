#ifndef RENDERTIMER_H
#define RENDERTIMER_H
#include <wx/wx.h>
#include <wx/timer.h>
#include "basicDrawPlane.h"


class RenderTimer : public wxTimer
{
public:
    RenderTimer(BasicDrawPlane* inPlane);
    void Notify();
    void start();
private:
    BasicDrawPlane* plane;
};

#endif // RENDERTIMER_H
