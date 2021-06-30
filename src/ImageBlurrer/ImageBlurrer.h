#ifndef IMAGEBLURRER_H
#define IMAGEBLURRER_H

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

    void run()
    {
        cout << "image blurrer running..." << endl; 
    }
};

#endif