#ifndef IMAGEBLURRER_APP_H
#define IMAGEBLURRER_APP_H

// include Configuration file
#include "ImageBlurrerHeader.h"

using namespace std;

class ImageBlurrerApp
{
private:
    vector<cv::Mat> originImages;
    vector<cv::Mat> blurredImages;
    ImageBlurrerInput *input;
    string origin;
    ImageFileParser *parser;
    ImageBlurrer *blurrer;
    bool blurCheck;
    int blurredImagesCount;

public:
    ImageBlurrerApp(string inputFile)
    {
        vector<cv::Mat> originImages();
        vector<cv::Mat> blurredImages();

        this->input = new ImageBlurrerInput(inputFile);
        this->origin = this->input->getOriginFilePath() + this->input->getOriginFileName();
        this->parser = new ImageFileParser(this->origin, this->input->getDigits(), this->input->getFirst(), this->input->getCount(), this->input->getExtension());
        this->blurrer = new ImageBlurrer(this->input->getBlurX(), this->input->getBlurY(), this->input->getBlurZ());
        this->blurCheck = (*this).check(); 
        if(blurCheck) (*this).readOriginImages();       
    }

    virtual ~ImageBlurrerApp()
    {
        if(this->blurrer != NULL)
        {
            delete this->blurrer;
            this->blurrer = NULL;
        }

        if(this->input != NULL)
        {
            delete this->input;
            this->input = NULL;
        }

        if(this->parser != NULL)
        {
            delete this->parser;
            this->parser = NULL;
        }
    }

    bool check()
    {
        /* 
            Check if origin images have uniform resolutions
        */
        string imageFile;
        cv::Mat *image;
        int width;
        int height;
        bool quit = false;

        imageFile = this->parser->next();
        image = new cv::Mat(cv::imread(imageFile));
        width = image->cols;
        height = image->rows;

        while(!quit)
        {
            try 
            {
                imageFile = this->parser->next();
                image = new cv::Mat(cv::imread(imageFile));
                if(image->cols != width || image->rows != height)
                {
                    cout << "image list is not uniform" << endl;
                    return false;
                }
            } catch (const std::invalid_argument& e) 
            {
                std::cerr << "exception: " << e.what() << std::endl; 
                quit = true;
            }
        } 
        this->parser->reset();
        
        /* 
            Check if origin image is divisible by blur factors in each direction
        */
        int blurX = this->blurrer->getBlurX();
        int blurY = this->blurrer->getBlurY();
        int blurZ = this->blurrer->getBlurZ();
        int count = this->parser->getCount();
                
        if(width % blurX != 0)
        { 
            cout << "blurX factor is invalid" << endl;
            return false;
        }

        if(height % blurY != 0)
        { 
            cout << "blurY factor is invalid" << endl;
            return false;
        }

        if(count % blurZ != 0)
        { 
            cout << "blurZ factor is invalid" << endl;
            return false;
        }

        return true;
    }

    void readOriginImages()
    {
        string imageFile;
        cv::Mat *image;
        bool quit = false;

        while(!quit)
        {
            try 
            {
                imageFile = this->parser->next();
                cout << "checking " << imageFile << endl;
                image = new cv::Mat(cv::imread(imageFile));
                this->originImages.push_back(image->clone());
            } catch (const std::invalid_argument& e) 
            {
                std::cerr << "exception: " << e.what() << std::endl; 
                quit = true;
            }
        } 
        this->parser->reset();
    }

    void run()
    {
        cout << "image blurrer app running..." << endl;
        vector<cv::Mat> currentImages;
        int currentIdx = 0;
        this->blurredImagesCount = this->originImages.size() / this->blurrer->getBlurZ();
        for(int imageIdx = 0; imageIdx < this->blurredImagesCount; imageIdx++)
        {
            
            for(int slice = 0; slice < this->blurrer->getBlurZ(); slice++)
            {
                currentIdx = imageIdx * this->blurrer->getBlurZ() + slice;
                cout << "reading origin image slice " << currentIdx << endl;
                currentImages.push_back(this->originImages[currentIdx]);
            }

            this->blurrer->run();
            currentImages.clear();
        }         
    }
};

#endif