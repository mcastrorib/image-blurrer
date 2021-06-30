#ifndef IMAGEBLURRER_HEADER_H
#define IMAGEBLURRER_HEADER_H

// include C++ standard libraries
#include <unistd.h>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <limits>
#include <random>
#include <omp.h>

// include opencv libraries
#include <opencv2/core.hpp>
#include <opencv2/core/utility.hpp>
#include "opencv2/imgcodecs.hpp"
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui.hpp>

// define macros

// include ImageBlurrer lib classes
#include "ImageBlurrer.h"
#include "ImageFileParser.h"
#include "ImageBlurrerInput.h"
#include "ImageBlurrerApp.h"

#endif