#ifndef IMAGEANALYZER_H
#define IMAGEANALYZER_H
#include <vector>


class ImageAnalyzer
{
public:
    ImageAnalyzer();
    void setImageData(unsigned char* inNewImage,
                      const unsigned int inWidth,
                      const unsigned int inHeight,
                      const unsigned int inBitDepth = 3);
    void setPiviotPoint(const unsigned int inPoint_X, const unsigned int inPoint_Y);
    void setAnalyzLength(const unsigned int inLength);
    void setAngleRange(const float inLowAngle, const float inHighAngle);
    void setAngleStep(const float inAngleStep);
    float getAngleStep()const;

    bool getIsRadialSeal()const;
    float getPartMylarRadius()const;
    float getPartYOffset()const;
    float getStartAngle()const;
    float getEndAngle()const;
    float getAngle()const;
    float getAngleNonDCT()const;

private:
    unsigned char* currentImage;
    unsigned int imageWidth;
    unsigned int imageHeight;
    unsigned int bitDepth;
    unsigned int piviotPoint_X;
    unsigned int piviotPoint_Y;
    unsigned int analyzLength;
    float lowAngle;
    float highAngle;
    float angleStep;



    std::vector<unsigned int> getVector(const float inAngle)const;
    std::vector<float> DCT(std::vector<unsigned int>* inData)const;
    unsigned char getPixelValue(const unsigned int inX, const unsigned int inY)const;
    float rotatePoint_X(const float inX, const float inY, const float inAngle)const;
    float rotatePoint_Y(const float inX, const float inY, const float inAngle)const;

};

#endif // IMAGEANALYZER_H
