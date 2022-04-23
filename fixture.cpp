#include "fixture.h"

Fixture::Fixture(const std::string inSerialNumber)
{
    serialNumber = inSerialNumber;
    isSubmitted = true;
}

bool Fixture::operator<(const Fixture& RHS)
{
    return (serialNumber.compare(RHS.serialNumber) < 0);
}

void Fixture::addDataPoint(FixtureData* inData, bool inIsSubmitted)
{
    listOfData.push_back(inData);
    isSubmitted = inIsSubmitted;
}

FixtureData* Fixture::getFixtureData(const unsigned int inLocation)const
{
    return listOfData.at(inLocation);
}

unsigned int Fixture::getDataSize()const
{
    return listOfData.size();
}

void Fixture::popData()
{
    if(!isSubmitted)
    {
        delete listOfData.at(listOfData.size() - 1);
        listOfData.pop_back();
        isSubmitted = true;
    }
}


std::string Fixture::getSerialNumber()const
{
    return serialNumber;
}

bool Fixture::getIsSubmitted()const
{
    return isSubmitted;
}

void Fixture::submit()
{
    if(!isSubmitted)
    {
        isSubmitted = true;
    }
}
