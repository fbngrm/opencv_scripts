#include "opencv2/highgui/highgui.hpp"
#include <iostream>

using namespace cv;
using namespace std;

Mat createLookupTable(int quantize){

  Mat lookUpTable(1, 256, CV_8U);
  uchar* p = lookUpTable.data;
  for( int i = 0; i < 256; ++i) {
    p[i] = uchar(i/quantize);
  }
  return lookUpTable;
}

Mat& reduce(Mat& frame, int q) {
  int channels = frame.channels();

  int nRows = frame.rows;
  int nCols = frame.cols * channels;
  unsigned char b;
  unsigned char g;
  unsigned char r;

  if (frame.isContinuous())
  {
    nCols *= nRows;
    nRows = 1;
  }

  int i,j;
  uchar* p;
  for( i = 0; i < nRows; ++i)
  {
    p = frame.ptr<uchar>(i);
    for ( j = 0; j < nCols; j+=3)
    {
      b = p[j]/q;
      g = p[j+1]/q;
      r = p[j+2]/q;

      p[j] = b;
      p[j+1] = g;
      p[j+2] = r;
    }
  }
return frame;
}

int main(int argc, char* argv[])
{

  if (argc<3) {
    cout << "not enough arguments supplied!" << endl;
    return -1;
  }

  VideoCapture cap(argv[1]); 

  if(!cap.isOpened())
  {
    cout << "cannot open the video file" << endl;
    return -1;
  }

  double dWidth = cap.get(CV_CAP_PROP_FRAME_WIDTH); 
  double dHeight = cap.get(CV_CAP_PROP_FRAME_HEIGHT); 
  cout << "frame size = " << dWidth << "x" << dHeight << endl;
  Size frameSize(static_cast<int>(dWidth), static_cast<int>(dHeight));

  VideoWriter oVideoWriter (argv[2], CV_FOURCC('P','I','M','1'), 20, frameSize, true); 

  if(!oVideoWriter.isOpened()) 
  {
    cout << "ERROR: failed to write the video" << endl;
    return -1;
  }

  Mat prev_frame;
  cap.read(prev_frame);

  while (1)
  {
    Mat frame;
    bool bSuccess = cap.read(frame); 

    if(!bSuccess)
    {
      cout << "ERROR: cannot read a frame from video file" << endl;
      break;
    }

    Mat dif = frame-prev_frame;
    imshow("differenzbild", dif);
    //oVideoWriter.write(dif);

    Mat dif_red = reduce(dif, 2);
    imshow("quantisiertes differenzbild", dif_red);

    prev_frame = frame.clone();

    if(waitKey(10) == 27)
    {
      cout << "esc key is pressed by user" << endl;
      break; 
    }
  }
  return 0;
}
