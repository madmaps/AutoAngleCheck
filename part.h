#ifndef PART_H
#define PART_H
#include <string>
#include "fixture.h"


class Part
{
public:
    Part(const std::string inPartName,
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
         const float inCaptureAngleEnd);
    ~Part();
    bool operator<(const Part& RHS);

    std::string getPartName()const;
    float getLowYellowAngle()const;
    float getLowGreenAngle()const;
    float getHighGreenAngle()const;
    float getHighYellowAngle()const;
    bool getIsRadialSeal()const;
    float getMylarRadius()const;
    float getYOffset()const;
    unsigned int getStartCaptureX()const;
    unsigned int getStartCaptureY()const;
    unsigned int getEndCaptureX()const;
    unsigned int getEndCaptureY()const;
    unsigned int getCaptureStep()const;
    unsigned int getCaptureRadius()const;
    float getCaptureAngleStep()const;
    float getCaptureStartAngle()const;
    float getCaptureEndAngle()const;
    std::vector<Fixture*> getFixtureList()const;
    void addFixture(Fixture* inFixture);
private:
    std::string partName;
    float lowYellowAngle;
    float lowGreenAngle;
    float highGreenAngle;
    float highYellowAngle;
    bool isRadialSeal;
    float mylarRadius;
    float yOffset;
    unsigned int startCaptureX;
    unsigned int startCaptureY;
    unsigned int endCaptureX;
    unsigned int endCaptureY;
    unsigned int captureStep;
    unsigned int captureRadius;
    float captureAngleStep;
    float captureStartAngle;
    float captureEndAngle;
    std::vector<Fixture*> fixtureList;
};

#endif // PART_H
