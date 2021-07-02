#ifndef IMAGEBLURRER_APP_H_
#define IMAGEBLURRER_APP_H_

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
            Check if origin images are not empty or have uniform resolutions
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

        if(width == 0 || height == 0 || this->parser->getCount() == 0)
        {
            cout << "file: " << imageFile << "is an empty image" << endl;
            return false;
        }

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
                image = new cv::Mat(cv::imread(imageFile));
                if (!image->data)
                {
                    cout << "Error: No image data in file " << imageFile << endl;
                    quit = true;
                } else  
                {
                    this->originImages.push_back(image->clone());
                }
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
        if(this->blurCheck)
        {
            /*
                Apply 'blur' to original image
            */
            cout << "image blurrer app running..." << endl;
            vector<cv::Mat> currentImages(0);
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

                try
                {
                    if(this->input->getBinarize()) 
                    {
                        blurredImages.push_back(this->blurrer->blurAndBinarize(currentImages, this->input->getThreshold()));
                    } else 
                    {
                        blurredImages.push_back(this->blurrer->blur(currentImages));
                    }
                    currentImages.clear();
                    cv::imshow("image", blurredImages[imageIdx]);
                    cv::waitKey(20);
                } catch (const std::invalid_argument& e)
                {
                    std::cerr << "exception: " << e.what() << std::endl; 
                    return;
                }
            }   

            /*
                If assigned, rescale image to original size
            */
            if(this->input->getResize())
            {
                ImageScale resizer(this->input->getScale(), this->blurredImages);
                this->blurredImages = resizer.getRescaledImages();
                this->blurredImagesCount *= this->input->getScale();
            }

            /*
                Save proccessed image
            */
            (*this).save();      
        } else
        {
                cout << "image list is not valid." << endl;
                return;
        }
    }

    void save()
    {
        (*this).createDestinationDir();   
        string fullpath = this->input->getDestinationFilePath() + this->input->getDestinationFileName() + "/" + this->input->getDestinationFileName();
        int digits = (*this).findDigits(this->blurredImagesCount);
        for(int slice = 0; slice < this->blurredImagesCount; slice++)
        {
            (*this).saveImageSlice(fullpath, slice, digits, this->input->getExtension());
        }
    }

    int findDigits(int n)
    {
        int count = 1;
        int result = n / 10;

        while (result > 0)
        {
            count++;
            result = result / 10;
        }

        return count;
    }

    void saveImageSlice(string path, int slice, int digits, string extension)
    {
        string fileIdx = (*this).convertCurrentIdxToString(slice, digits);
        string fullpath = path + "_" + fileIdx + extension;
        cv::imwrite(fullpath, this->blurredImages[slice]); 
    }

    string convertCurrentIdxToString(int idx, int digits)
    {
        stringstream result;
        result << std::setfill('0') << std::setw(digits) << idx;
        return result.str();
    }

    void createDestinationDir()
    {
        string path = this->input->getDestinationFilePath() + this->input->getDestinationFileName();
        char directory[path.size() + 1];
        strcpy(directory, path.c_str());

        struct stat st = {0};

        if (stat(directory, &st) == -1)
        {
            mkdir(directory, 0700);
        } 
    }
};

#endif