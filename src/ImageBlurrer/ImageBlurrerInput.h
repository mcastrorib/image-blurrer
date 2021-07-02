#ifndef IMAGEBLURRERINPUT_H_
#define IMAGEBLURRERINPUT_H_

// include Configuration file
#include "ImageBlurrerHeader.h"

using namespace std;

class ImageBlurrerInput
{
private:
    string inputFile;
    string originFilePath;
    string originFileName;
    string destinationFilePath;
    string destinationFileName;
    string extension;
    int digits;
    int first;
    int count;
    int blurX;
    int blurY;
    int blurZ;
    bool binarize;
    uchar threshold;
    bool resize;
    int scale;

public:
    ImageBlurrerInput(const string inputFile)
    {
        this->inputFile = inputFile;
        (*this).readInputFile();
    }

    ImageBlurrerInput(const ImageBlurrerInput &other)
    {
        this->inputFile = other.inputFile;
        this->originFilePath = other.originFilePath;
        this->originFileName = other.originFileName;
        this->destinationFilePath = other.destinationFilePath;
        this->destinationFileName = other.destinationFileName;
        this->extension = other.extension;
        this->digits = other.digits;
        this->first = other.first;
        this->count = other.count;
        this->blurX = other.blurX;
        this->blurY = other.blurY;
        this->blurZ = other.blurZ;
        this->binarize = other.binarize;
        this->threshold = other.threshold;
        this->resize = other.resize;
        this->scale = other.scale;

    }

    virtual ~ImageBlurrerInput()
    {}

    string getOriginFilePath() { return this->originFilePath; }
    string getOriginFileName() { return this->originFileName; }
    string getDestinationFilePath() { return this->destinationFilePath; }
    string getDestinationFileName() { return this->destinationFileName; }
    string getExtension() { return this->extension; }
    int getDigits() { return this->digits; }
    int getFirst() { return this->first; }
    int getCount() { return this->count; }
    int getBlurX() { return this->blurX; }
    int getBlurY() { return this->blurY; }
    int getBlurZ() { return this->blurZ; }
    bool getBinarize() { return this->binarize; }
    uchar getThreshold() { return this->threshold; }
    bool getResize() { return this->resize; }
    int getScale() { return this->scale; }



    void readInputFile()
    {
        ifstream fileObject;
        fileObject.open(this->inputFile, ios::in);
        if (fileObject.fail())
        {
            cout << "Could not open uct config file from disc." << endl;
            exit(1);
        }

        string line;
        while(fileObject)
        {
            getline(fileObject, line);
            // cout << line << endl;

            string s = line;
            string delimiter = ": ";
            size_t pos = 0;
            string token, content;
            while ((pos = s.find(delimiter)) != std::string::npos) 
            {
                token = s.substr(0, pos);
                content = s.substr(pos + delimiter.length(), s.length());
                s.erase(0, pos + delimiter.length());

                if(token == "ORIGIN_PATH") (*this).readOriginFilePath(content);
                else if(token == "ORIGIN_FILE") (*this).readOriginFileName(content);
                else if(token == "DESTINATION_PATH") (*this).readDestinationFilePath(content);
                else if(token == "DESTINATION_FILE") (*this).readDestinationFileName(content);
                else if(token == "EXTENSION") (*this).readExtension(content);
                else if(token == "DIGITS") (*this).readDigits(content);
                else if(token == "FIRST") (*this).readFirst(content);
                else if(token == "COUNT") (*this).readCount(content);
                else if(token == "BLUR_X") (*this).readBlurX(content);
                else if(token == "BLUR_Y") (*this).readBlurY(content);
                else if(token == "BLUR_Z") (*this).readBlurZ(content);
                else if(token == "BINARIZE") (*this).readBinarize(content);
                else if(token == "THRESHOLD") (*this).readThreshold(content);
                else if(token == "RESIZE") (*this).readResize(content);
                else if(token == "SCALE") (*this).readScale(content);                          
            }
        } 

        fileObject.close();
    }

    void readOriginFilePath(string content)
    {
        this->originFilePath = content;
    }

    void readOriginFileName(string content)
    {
        this->originFileName = content;
    }

    void readDestinationFilePath(string content)
    {
        this->destinationFilePath = content;
    }

    void readDestinationFileName(string content)
    {
        this->destinationFileName = content;
    }

    void readExtension(string content)
    {
        this->extension = content;
    }

    void readDigits(string content)
    {
        this->digits = std::stoi(content);
    }

    void readFirst(string content)
    {
        this->first = std::stoi(content);
    }

    void readCount(string content)
    {
        this->count = std::stoi(content);
    }

    void readBlurX(string content)
    {
        this->blurX = std::stoi(content);
    }

    void readBlurY(string content)
    {
        this->blurY = std::stoi(content);
    }

    void readBlurZ(string content)
    {
        this->blurZ = std::stoi(content);
    }

    void readBinarize(string content)
    {
        if(content == "True" || content == "true") this->binarize = true;
        else this->binarize = false;
    }

    void readThreshold(string content)
    {
        int a = std::stoi(content);
        this->threshold = (uchar) a;
    }

    void readResize(string content)
    {
        if(content == "True" || content == "true") this->resize = true;
        else this->resize = false;
    }    

    void readScale(string content)
    {
        this->scale = std::stoi(content);
    }

};

#endif