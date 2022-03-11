#include "renderTimer.h"

RenderTimer::RenderTimer(BasicDrawPlane* inPlane) : wxTimer()
{
    plane = inPlane;
}

void RenderTimer::Notify()
{
    plane->Refresh();
}

void RenderTimer::start()
{
    wxTimer::Start(10);
}
