#include "fixtureData.h"

FixtureData::FixtureData(const std::string& inSerialNumber, const float inAngleValue, const std::string& inComment, const std::chrono::time_point<std::chrono::system_clock>& inDateTime)
{
    serialNumber = inSerialNumber;
    angleValue = inAngleValue;
    comment = inComment;
    dateTime = inDateTime;
}

std::string FixtureData::getSerialNumber()const
{
    return serialNumber;
}

float FixtureData::getAngleValue()const
{
    return angleValue;
}

