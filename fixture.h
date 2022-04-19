#ifndef FIXTURE_H
#define FIXTURE_H
#include <string>
#include <vector>
#include "fixtureData.h"


class Fixture
{
public:
    Fixture(const std::string inSerialNumber);
    bool operator<(const Fixture& RHS);
    void addDataPoint(FixtureData* inData, bool inIsSubmitted = true);
    FixtureData* getFixtureData(const unsigned int inLocation)const;
    unsigned int getDataSize()const;
    std::string getSerialNumber()const;
    bool getIsSubmitted()const;
    void submit();
private:
    std::string serialNumber;
    std::vector<FixtureData*> listOfData;
    bool isSubmitted;
};

#endif // FIXTURE_H
