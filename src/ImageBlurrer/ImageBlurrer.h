#ifndef IMAGEBLURRER_H_
#define IMAGEBLURRER_H_

// include Configuration file
#include "ImageBlurrerHeader.h"

using namespace std;

class ImageBlurrer
{
private:
    int blurX;
    int blurY;
    int blurZ;

public:
    ImageBlurrer(int blurX, int blurY, int blurZ)
    {
        this->blurX = blurX;
        this->blurY = blurY;
        this->blurZ = blurZ;
    }

    virtual ~ImageBlurrer()
    {}

    int getBlurX() { return this->blurX; }
    int getBlurY() { return this->blurY; }
    int getBlurZ() { return this->blurZ; }

    cv::Mat blur(vector<cv::Mat> &originImages)
    {
        int originSlices = originImages.size();
        if(originSlices > 0)
        {
            int originChannels = originImages[0].channels();
            int originRows = originImages[0].rows;
            int originCols = originImages[0].cols;
            int blurredRows = originRows / this->getBlurY();
            int blurredCols = originCols / this->getBlurX();
            cv::Mat blurredImage = cv::Mat::zeros(blurredRows, blurredCols, originImages[0].type());
            int blurredChannels = blurredImage.channels();

            
            /*
                walk through blurred image pixels
            */
            double cubeVol = (double) (*this).getBlurX() * (*this).getBlurY() * (*this).getBlurZ(); 
            uchar *currentPixel;
            uchar *blurredPixel;
            int *newPixelValue = new int[originChannels];
            for(int row = 0; row < blurredRows; row++)
            {
                for(int col = 0; col < blurredCols; col++)
                {
                    /*
                        Reset new values arrays 
                    */
                    for(int bchannel = 0; bchannel < originChannels; bchannel++)
                    {
                        newPixelValue[bchannel] = 0;
                    }

                    /*
                        Walk through little cubes inside origin images 
                    */    
                    int pixelX;
                    int pixelY;
                    int pixelZ;
                    for(int cubeZ = 0; cubeZ < (*this).getBlurZ(); cubeZ++)
                    {
                        for(int cubeY = 0; cubeY < (*this).getBlurY(); cubeY++)
                        {
                            for(int cubeX = 0; cubeX < (*this).getBlurX(); cubeX++)
                            {
                                pixelZ = cubeZ;
                                pixelY = row * (*this).getBlurY() + cubeY;
                                currentPixel = originImages[pixelZ].ptr<uchar>(pixelY);
                                
                                for(int channel = 0; channel < originChannels; channel++)
                                {
                                    pixelX = originChannels * (col * (*this).getBlurX() + cubeX) + channel;
                                    newPixelValue[channel] += (int) currentPixel[pixelX];
                                }  
                            }
                        }                        
                    }

                    /*
                        Update blurred image pixels from average value arrays 
                    */
                    blurredPixel = blurredImage.ptr<uchar>(row);
                    for(int bchannel = 0; bchannel < originChannels; bchannel++)
                    {
                        newPixelValue[bchannel] = (int) round(newPixelValue[bchannel] / cubeVol);
                        blurredPixel[(col * originChannels) + bchannel] = (uchar) newPixelValue[bchannel];
                    }
                }
            }

            return blurredImage; 
        } else
        {
            throw std::invalid_argument("origin images is empty");
        }
    }

    cv::Mat blurAndBinarize(vector<cv::Mat> &originImages, uchar _threshold=126)
    {
        int originSlices = originImages.size();
        if(originSlices == (*this).getBlurZ())
        {
            int originChannels = originImages[0].channels();
            int originRows = originImages[0].rows;
            int originCols = originImages[0].cols;
            int blurredRows = originRows / this->getBlurY();
            int blurredCols = originCols / this->getBlurX();
            cv::Mat blurredImage = cv::Mat::zeros(blurredRows, blurredCols, CV_8UC1);
            int blurredChannels = blurredImage.channels();

            /*
                walk through blurred image pixels
            */
            double cubeVol = (double) (*this).getBlurX() * (*this).getBlurY() * (*this).getBlurZ(); 
            uchar *currentPixel;
            uchar *blurredPixel;
            int newPixelValue;
            int pixelsAboveThreshold;
            int pixelsBelowThreshold;
            mRNG randomDrawer(0,1);
            
            for(int row = 0; row < blurredRows; row++)
            {
                for(int col = 0; col < blurredCols; col++)
                {
                    /*
                        Reset new values arrays 
                    */
                    newPixelValue = 0;
                    pixelsAboveThreshold = 0;
                    pixelsBelowThreshold = 0;
                    

                    /*
                        Walk through little cubes inside origin images comparing pixels with threshold
                    */    
                    int pixelX;
                    int pixelY;
                    int pixelZ;
                    for(int cubeZ = 0; cubeZ < (*this).getBlurZ(); cubeZ++)
                    {
                        for(int cubeY = 0; cubeY < (*this).getBlurY(); cubeY++)
                        {
                            for(int cubeX = 0; cubeX < (*this).getBlurX(); cubeX++)
                            {
                                newPixelValue = 0;
                                pixelZ = cubeZ;
                                pixelY = row * (*this).getBlurY() + cubeY;
                                currentPixel = originImages[pixelZ].ptr<uchar>(pixelY);
                                
                                for(int channel = 0; channel < originChannels; channel++)
                                {
                                    pixelX = originChannels * (col * (*this).getBlurX() + cubeX) + channel;
                                    newPixelValue += (int) currentPixel[pixelX];
                                }
                                
                                newPixelValue = round(newPixelValue / (double) originChannels);
                                if(newPixelValue > _threshold)
                                {
                                    pixelsAboveThreshold++;
                                } else if(newPixelValue < _threshold)
                                {
                                    pixelsBelowThreshold++;
                                }
                            }
                        }                        
                    }

                    /*
                        Update blurred image pixels from average value arrays 
                    */
                    blurredPixel = blurredImage.ptr<uchar>(row);
                    if(pixelsAboveThreshold < pixelsBelowThreshold)
                    {
                        blurredPixel[col] = 0;
                    } else 
                    {
                        blurredPixel[col] = 127;
                    } 
                    // else
                    // {
                    //     cout << row << ", " << col << " had a draw :(" << endl;
                    //     if(randomDrawer()) blurredPixel[col] = 2 * _threshold;
                    //     else blurredPixel[col] = 2 * _threshold;
                    // }                    
                }
            }

            return blurredImage; 
        } else
        {
            throw std::invalid_argument("origin images is empty");
        }
    }
};

#endif