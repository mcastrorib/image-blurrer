#ifndef IMAGESCALE_H_
#define IMAGESCALE_H_

#include "ImageBlurrerHeader.h"

using namespace std;

class ImageScale
{
private:
    int scale;
    vector<cv::Mat> rescaledImages;
public:
    ImageScale(int scale, vector<cv::Mat> &originImages)
    {
        vector<cv::Mat> rescaledImages();
        this->scale = scale;
        (*this).rescale(originImages);
    }

    virtual ~ImageScale()
    {}

    int getScale() { return this->scale; }
    vector<cv::Mat> getRescaledImages() { return this->rescaledImages; }

    void rescale(vector<cv::Mat> &originImages)
    {
        cout << "rescaling image to be implemented." << endl;
        if(this->rescaledImages.size() > 0) this->rescaledImages.clear();
        int originSlices = originImages.size();
        
        if(originSlices > 0)
        {
            int originChannels = originImages[0].channels();
            int originRows = originImages[0].rows;
            int originCols = originImages[0].cols;
            
            int rescaledChannels = originChannels;
            int rescaledRows = originRows * this->scale;
            int rescaledCols = originCols * this->scale;
            int rescaledSlices = originSlices * this->scale;
            
            uchar *originPixel;
            uchar *rescaledPixel;
            for(int slice = 0; slice < originSlices; slice++)
            {
                cv::Mat newImage = cv::Mat::zeros(rescaledRows, rescaledCols, originImages[slice].type());

                for(int row = 0; row < originRows; row++)
                {
                    
                    originPixel = originImages[slice].ptr<uchar>(row);
                    
                    for(int col = 0; col < originCols; col++)
                    {
                        for(int channel = 0; channel < originChannels; channel++)
                        {
                            
                            uchar originPixelValue = originPixel[(col*originChannels) + channel];

                            for(int scaledY = 0; scaledY < this->scale; scaledY++)
                            {

                                rescaledPixel = newImage.ptr<uchar>((row * this->scale) + scaledY);

                                for(int scaledX = 0; scaledX < this->scale; scaledX++)
                                {
                                    for(int scaledChannel = 0; scaledChannel < rescaledChannels; scaledChannel++)
                                    {
                                        int localIdx = (col * rescaledChannels * this->scale) + scaledX * rescaledChannels + scaledChannel;
                                        rescaledPixel[localIdx] = originPixelValue;
                                    }
                                }    
                            }
                        }
                    }
                }

                for(int rescaledSlice = 0; rescaledSlice < this->scale; rescaledSlice++)
                {
                    this->rescaledImages.push_back(newImage);
                }

            }
        }
        
    }

};

#endif