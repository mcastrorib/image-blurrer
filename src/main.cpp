// include CMake Configuration file
#include "imageblurrer_config.h"

// include built-in PathFinder library
#include "ImageBlurrerHeader.h"

// Main Program
int main(int argc, char *argv[])
{        
    ImageBlurrerApp *app;
    app = new ImageBlurrerApp(INPUT_FILE);
    app->run();
    
    delete app;
    app = NULL;
    return 0;
}
