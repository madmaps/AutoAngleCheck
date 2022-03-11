#ifndef CAPP_H
#define CAPP_H
#include <wx/wx.h>
#include "cMain.h"



class cApp : public wxApp
{
public:
    cApp();
    ~cApp();
    virtual bool OnInit();

private:
    cMain* myMainClass;
};

#endif // CAPP_H
