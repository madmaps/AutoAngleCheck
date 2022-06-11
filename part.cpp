#include "part.h"
#include <algorithm>


bool cmpTwoFixtures(Fixture* fixtureOne, Fixture* fixtureTwo)
{
    return (*fixtureOne < *fixtureTwo);
}

Part::Part(const std::string inPartName,
           const float inLowYellowAngle,
           const float inLowGreenAngle,
           const float inHighGreenAngle,
           const float inHighYellowAngle,
           const bool inIsRadialSeal,
           const float inMylarRadius,
           const float inYOffset,
           const unsigned int inStartCaptureX,
           const unsigned int inStartCaptureY,
           const unsigned int inEndCaptureX,
           const unsigned int inEndCaptureY,
           const unsigned int inCaptureStep,
           const unsigned int inCaptureRadius,
           const float inCaptureAngleStep,
           const float inCaptureAngleStart,
           const float inCaptureAngleEnd,
           const unsigned int inMinCaptureAmount) :
           partName(inPartName),
           lowYellowAngle(inLowYellowAngle),
           lowGreenAngle(inLowGreenAngle),
           highGreenAngle(inHighGreenAngle),
           highYellowAngle(inHighYellowAngle),
           isRadialSeal(inIsRadialSeal),
           mylarRadius(inMylarRadius),
           yOffset(inYOffset),
           startCaptureX(inStartCaptureX),
           startCaptureY(inStartCaptureY),
           endCaptureX(inEndCaptureX),
           endCaptureY(inEndCaptureY),
           captureStep(inCaptureStep),
           captureRadius(inCaptureRadius),
           captureAngleStep(inCaptureAngleStep),
           captureStartAngle(inCaptureAngleStart),
           captureEndAngle(inCaptureAngleEnd),
           minCaptureAmount(inMinCaptureAmount)

{

}

Part::~Part()
{
    for(Fixture* deleteMe : fixtureList)
    {
        delete deleteMe;
        deleteMe = 0;
    }
    fixtureList.clear();
}


bool Part::operator<(const Part& RHS)
{
    return (partName.compare(RHS.partName) < 0);
}


std::string Part::getPartName()const
{
    return partName;
}

float Part::getLowYellowAngle()const
{
    return lowYellowAngle;
}

float Part::getLowGreenAngle()const
{
    return lowGreenAngle;
}

float Part::getHighGreenAngle()const
{
    return highGreenAngle;
}

float Part::getHighYellowAngle()const
{
    return highYellowAngle;
}

bool Part::getIsRadialSeal()const
{
    return isRadialSeal;
}

float Part::getMylarRadius()const
{
    return mylarRadius;
}

float Part::getYOffset()const
{
    return yOffset;
}

unsigned int Part::getStartCaptureX()const
{
    return startCaptureX;
}

unsigned int Part::getStartCaptureY()const
{
    return startCaptureY;
}

unsigned int Part::getEndCaptureX()const
{
    return endCaptureX;
}

unsigned int Part::getEndCaptureY()const
{
    return endCaptureY;
}

unsigned int Part::getCaptureStep()const
{
    return captureStep;
}

unsigned int Part::getCaptureRadius()const
{
    return captureRadius;
}

float Part::getCaptureAngleStep()const
{
    return captureAngleStep;
}

float Part::getCaptureStartAngle() const
{
    return captureStartAngle;
}

float Part::getCaptureEndAngle()const
{
    return captureEndAngle;
}

unsigned int Part::getMinCaptureAmount()const
{
    return minCaptureAmount;
}
std::vector<Fixture*> Part::getFixtureList()const
{
    return fixtureList;
}

void Part::addFixture(Fixture* inFixture)
{
    fixtureList.push_back(inFixture);
    sort(fixtureList.begin(), fixtureList.end(), cmpTwoFixtures);
}


