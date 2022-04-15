#include "fixture.h"

Fixture::Fixture(const std::string inSerialNumber)
{
    serialNumber = inSerialNumber;
}

bool Fixture::operator<(const Fixture& RHS)
{
    return (serialNumber.compare(RHS.serialNumber) < 0);
}

void Fixture::addDataPoint(FixtureData* inData)
{
    listOfData.push_back(inData);
}

FixtureData* Fixture::getFixtureData(const unsigned int inLocation)const
{
    return listOfData.at(inLocation);
}

unsigned int Fixture::getDataSize()const
{
    return listOfData.size();
}

std::string Fixture::getSerialNumber()const
{
    return serialNumber;
}
