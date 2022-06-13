#include "imageanalyzer.h"
#include <math.h>
#include <iostream>

using namespace std;
ImageAnalyzer::ImageAnalyzer()
{
    currentImage = nullptr;
    imageWidth = 0;
    imageHeight = 0;
    bitDepth = 0;
    piviotPoint_X = 0;
    piviotPoint_Y = 0;
    analyzLength = 64;
    lowAngle = 0;
    highAngle = 360;
    angleStep = 1;
}

void ImageAnalyzer::setImageData(unsigned char* inNewImage,
                                 const unsigned int inWidth,
                                 const unsigned int inHeight,
                                 const unsigned int inBitDepth)
{
    currentImage = inNewImage;
    imageWidth = inWidth;
    imageHeight = inHeight;
    bitDepth = inBitDepth;
}

void ImageAnalyzer::setPiviotPoint(const unsigned int inPoint_X, const unsigned int inPoint_Y)
{
        piviotPoint_X = inPoint_X;
        piviotPoint_Y = inPoint_Y;
}

float ImageAnalyzer::getStartAngle() const
{
    return lowAngle;
}

float ImageAnalyzer::getEndAngle() const
{
    return highAngle;
}

void ImageAnalyzer::setAngleRange(const float inLowAngle, const float inHighAngle)
{
    lowAngle = inLowAngle;
    highAngle = inHighAngle;
}

void ImageAnalyzer::setAnalyzLength(const unsigned int inLength)
{
        analyzLength = inLength;
}

void ImageAnalyzer::setAngleStep(const float inAngleStep)
{
    angleStep = inAngleStep;
}

float ImageAnalyzer::getAngleStep() const
{
    return angleStep;
}



std::vector<unsigned int> ImageAnalyzer::getVector(const float inAngle)const
{
    std::vector<unsigned int> returnVector;
    if(currentImage != nullptr)
    {
        float rotatedX = 0;
        float rotatedY = 0;
        for(int i = 0; i < analyzLength; i++)
        {
            rotatedY = -i;
            rotatedX = rotatePoint_X(0, rotatedY,inAngle);
            rotatedY = rotatePoint_Y(0, rotatedY,inAngle);
            rotatedX += piviotPoint_X;
            rotatedY += piviotPoint_Y;
            float lowXValue = (getPixelValue(ceil(rotatedX),floor(rotatedY)) -
                              getPixelValue(floor(rotatedX),floor(rotatedY))) *
                              (rotatedX - floor(rotatedX)) + getPixelValue(floor(rotatedX),
                              floor(rotatedY));
            float highXValue = (getPixelValue(ceil(rotatedX), ceil(rotatedY)) -
                               getPixelValue(floor(rotatedX), ceil(rotatedY))) *
                               (rotatedX - floor(rotatedX)) + getPixelValue(floor(rotatedX),
                               ceil(rotatedY));
            float value = (highXValue - lowXValue) * (rotatedY - floor(rotatedY)) + lowXValue;
            returnVector.push_back(value);
        }
    }
    return returnVector;
}

float ImageAnalyzer::getAngle()const
{
    std::vector<unsigned int> results;
    std::vector<float> DCTResults;
    float goodLowValue = 999999;
    float goodLowAngle = 0;
    float j = lowAngle;
    while(j <= highAngle)
    {
        results = getVector(j);

        DCTResults = DCT(&results);
        float freqValue = 0;
        for(unsigned int i = 1; i < DCTResults.size(); i++)
        {
            freqValue += fabs(DCTResults.at(i));
        }
        freqValue /= (analyzLength - 1);
        if(freqValue < goodLowValue)
        {
            goodLowValue = freqValue;
            goodLowAngle = j;
        }
        results.clear();
        DCTResults.clear();
        j += angleStep;
    }
    return goodLowAngle;
}

float ImageAnalyzer::getAngleNonDCT()const
{
    std::vector<unsigned int> results;
    float goodLowValue = 999999;
    float goodLowAngle = 0;
    float j = lowAngle;
    while(j <= highAngle)
    {
        results = getVector(j);
        unsigned int sumOfPixels = 0;
        for(unsigned int currentPixel : results)
        {
            sumOfPixels += currentPixel;
        }
        float average = sumOfPixels / results.size();
        float difference = 0;
        for(unsigned int currentPixel : results)
        {
            difference += fabs((float)currentPixel - average);
        }
        if(difference < goodLowValue)
        {
            goodLowValue = difference;
            goodLowAngle = j;
        }
        j += angleStep;
    }
    return goodLowAngle;
}

float ImageAnalyzer::getPixelValue(const unsigned int inX, const unsigned int inY)const
{
    float value = 0;
    for(unsigned int i = 0; i < bitDepth; i++)
    {
        value += currentImage[(inY * imageWidth * bitDepth) + (inX * bitDepth) + i];
    }
    value /= bitDepth;
    return value;
}


float ImageAnalyzer::rotatePoint_X(const float inX, const float inY, const float inAngle)const
{
    float radianAngle = (3.141592653589 * (inAngle)) / 180;
    return inX * cos(radianAngle) - inY * sin(radianAngle);
}
float ImageAnalyzer::rotatePoint_Y(const float inX, const float inY, const float inAngle)const
{
    float radianAngle = (3.141592653589 * (inAngle)) / 180;
    return inX * sin(radianAngle) + inY * cos(radianAngle);
}

std::vector<float> ImageAnalyzer::DCT(std::vector<unsigned int>* inData)const
{
    std::vector<float> returnData;
    float value = 0;
    for(unsigned int i = 0; i < inData->size(); i++)
    {
        value = 0;
        for(unsigned int j = 0; j < inData->size(); j++)
        {
            value += (float)inData->at(j) * cos(((((float)j + 0.5) * i) * 3.14159265358979) / inData->size());
        }
        returnData.push_back(value);
    }
    return returnData;
}

