#include "cMain.h"
#include <iostream>

using namespace std;

cMain::cMain() : wxFrame(nullptr, wxID_ANY, "Auto Angle Checker")
{
    myImageAnal = new ImageAnalyzer();
    myImageAnal->setAngleRange(270 + 360 , 90 + 360*2);
    goodCamera = true;
    camera.open(0);
    camera.set(cv::CAP_PROP_FRAME_WIDTH , 640);
    camera.set(cv::CAP_PROP_FRAME_HEIGHT, 480);

    if(!camera.isOpened())
    {
       goodCamera = false;
    }
    for(unsigned int i = 0; i < 1000; i++)
    {
        camera >> frame;
        myImageAnal->setImageData(frame.data,frame.cols,frame.rows);
        myImageAnal->setPiviotPoint(frame.cols / 2, frame.rows / 2);
        cout << myImageAnal->getAngle() - 720 << endl;
        //cout << i;
    }

}

cMain::~cMain()
{

}
