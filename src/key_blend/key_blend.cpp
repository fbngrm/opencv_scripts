#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>

using namespace cv;
using namespace std;

int main(int argc, char* argv[])
{
    if(argc<3) 
    {
        cout<<"too few arguments"<<endl;
        cout<<"use: open_webcam image_to_blend alpha"<<endl;
        return 0;
    }

    cout<<" simple linear blender "<<endl;
    cout<<"-----------------------"<<endl;

    double alpha = 0.5; 
    double beta; 
    double input;
    Mat img, cam, dst;

    // get alpha from user input
    if(atof(argv[2])>=0.0 && atof(argv[2])<=1.0) 
    {
        alpha = atof(argv[2]); 
    }
    else 
    {
        cout << "ERROR: alpha must be > 0.0 && < 1.0"
        return -1;
    }

    // read image ( same size, same type )
    img = imread(argv[1]);
    // check if image data exists
    if(!img.data) 
    { 
        cout<<"ERROR: loading image "<<endl; 
        return -1; 
    }

    // oprn webcam
    VideoCapture cap(0);
    // check if webcam is opened
    if (!cap.isOpened())
    {
        cout<<"ERROR: cannot open the webcam"<<endl;
        return -1;
    }

    // create a named window to show the video blend
    namedWindow("simple linear blend", CV_WINDOW_AUTOSIZE);

    // get the beta from input alpha
    beta = (1.0-alpha);

    // get dimensions of the video
    double dWidth = cap.get(CV_CAP_PROP_FRAME_WIDTH);
    double dHeight = cap.get(CV_CAP_PROP_FRAME_HEIGHT);

    cout << "frame size = " << dWidth << "x" << dHeight << endl;

    // create size from video dimensions
    Size frameSize(static_cast<int>(dWidth), static_cast<int>(dHeight));

    // resize the image to the size of the webcam resolution
    resize(img, img, frameSize);
    // show image window
    imshow("background image", img);   

    // get input-frames from webcam 
    while (1) {
        //read the next frame 
        bool bSuccess = cap.read(cam); 
        // break loop if there is noch input frame
        if (!bSuccess) 
        {
            cout<<"ERROR: cannot read a frame from video file"<<endl;
            break;
        }
        // blend image and video frame
        addWeighted(img, alpha, cam, beta, 0.0, dst);
        // show the blended video and image
        imshow("simple linear blend", dst);

        // break if "ESC" is pressed
        if (waitKey(10)==27)
        {
            cout<<"esc key is pressed by user"<<endl;
            break; 
        }
    }
    return 0;
}
