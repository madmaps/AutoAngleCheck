#ifndef FIXTUREDATA_H
#define FIXTUREDATA_H
#include <string>
#include <chrono>


class FixtureData
{
public:
    FixtureData(const std::string& inSerialNumber, const float inAngleValue, const std::string& inComment, const std::chrono::time_point<std::chrono::system_clock>& inDateTime);
    std::string getSerialNumber()const;
    float getAngleValue()const;
private:
    std::string serialNumber;
    float angleValue;
    std::string comment;
    std::chrono::time_point<std::chrono::system_clock> dateTime;
};

#endif // FIXTUREDATA_H
