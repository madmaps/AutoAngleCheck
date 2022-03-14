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
    partDiameter = 10;
    partYOffset = 3;
}

void ImageAnalyzer::setImageData(unsigned char* inNewImage,const unsigned int inWidth, const unsigned int inHeight,const unsigned int inBitDepth)
{
    currentImage = inNewImage;
    imageWidth = inWidth;
    imageHeight = inHeight;
    bitDepth = inBitDepth;
}

void ImageAnalyzer::setPiviotPoint(const unsigned int inPoint_X, const unsigned int inPoint_Y)
{
    //if(inPoint_X >= 0 && inPoint_X <= imageWidth && inPoint_Y >= 0 && inPoint_Y <= imageHeight)
    //{
        piviotPoint_X = inPoint_X;
        //piviotPoint_Y = imageHeight - inPoint_Y;
        piviotPoint_Y = inPoint_Y;
    //}
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

std::vector<unsigned int> ImageAnalyzer::getVector(const float inAngle)const
{
    std::vector<unsigned int> returnVector;
    if(currentImage != nullptr)
    {
        std::vector<float> SurroundingPoints = {0,0,0,0,0,0,0,0};
        float pointX = 0;
        float pointY = 0;
        float rotatedX = 0;
        float rotatedY = 0;
        for(int i = 0; i < analyzLength; i++)
        {
            pointX = 0;
            pointY = i;
            rotatedX = rotatePoint_X(pointX,pointY,inAngle);
            rotatedY = rotatePoint_Y(pointX,pointY,inAngle);
            rotatedX += piviotPoint_X;
            rotatedY += piviotPoint_Y;
            SurroundingPoints[0] = floor(rotatedX);
            SurroundingPoints[1] = ceil(rotatedY);
            SurroundingPoints[2] = ceil(rotatedX);
            SurroundingPoints[3] = ceil(rotatedY);
            SurroundingPoints[4] = ceil(rotatedX);
            SurroundingPoints[5] = floor(rotatedY);
            SurroundingPoints[6] = floor(rotatedX);
            SurroundingPoints[7] = floor(rotatedY);
            float lowXValue = ((float)getPixelValue((int)SurroundingPoints[4],(int)SurroundingPoints[5]) - (float)getPixelValue((int)SurroundingPoints[6],(int)SurroundingPoints[7])) * (rotatedX - floor(rotatedX)) + (float)getPixelValue((int)SurroundingPoints[6],(int)SurroundingPoints[7]);
            float highXValue = ((float)getPixelValue((int)SurroundingPoints[2],(int)SurroundingPoints[3]) - (float)getPixelValue((int)SurroundingPoints[0],(int)SurroundingPoints[1])) * (rotatedX - floor(rotatedX)) + (float)getPixelValue((int)SurroundingPoints[0],(int)SurroundingPoints[1]);
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
    //cout << "goodAngle: " << 360 * 2 - goodLowAngle << endl;
    //correctAngle(360 * 2 - goodLowAngle);
    return goodLowAngle;
    //return correctAngle(goodLowAngle);
}

unsigned char ImageAnalyzer::getPixelValue(const unsigned int inX, const unsigned int inY)const
{
    float value = 0;
    for(unsigned int i = 0; i < bitDepth; i++)
    {
        value += currentImage[(inY * imageWidth * bitDepth) + (inX * bitDepth)];
    }
    return (unsigned char)(value /= bitDepth);
}


float ImageAnalyzer::rotatePoint_X(const float inX, const float inY, const float inAngle)const
{
    float radianAngle = (3.141592653589 * (360 - inAngle)) / 180;
    return inX * cos(radianAngle) - inY * sin(radianAngle);
}
float ImageAnalyzer::rotatePoint_Y(const float inX, const float inY, const float inAngle)const
{
    float radianAngle = (3.141592653589 * (360 - inAngle)) / 180;
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

float ImageAnalyzer::correctAngle(float inAngle)const
{
    if(inAngle > 180)
    {
        inAngle -= 180;
    }
    float correctValue = -1;
    bool negInAngle = false;
    if(inAngle < 0)
    {
        negInAngle = true;
        inAngle = fabs(inAngle);
    }
    if(inAngle !=0 && inAngle != 180)
    {
        float R = 0;
        float decimalPoint = 1;
        correctValue = 180 - inAngle;
        float K = 9.5;
        while(decimalPoint > .0001)
        {
            R = K * sin(correctValue * M_PI / 180) + K * cos(correctValue * M_PI / 180) * tan((180 - (correctValue + inAngle)) * M_PI / 180);
            if(R > partDiameter)
            {
                correctValue += decimalPoint;
                decimalPoint /= 10;
            }
            else
            {
                correctValue -= decimalPoint;
            }
        }
        //cout << 90 - (180 - (correctValue + inAngle)) << endl;
    }
    if(negInAngle)
    {
        correctValue = -(90 - (180 - (correctValue + inAngle)));
    }
    else
    {
        correctValue = (90 - (180 - (correctValue + inAngle)));
    }
    return correctValue;
}
