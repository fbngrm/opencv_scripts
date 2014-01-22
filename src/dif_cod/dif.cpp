#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <cstdlib>

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
      b = abs(p[j]/q);
      g = abs(p[j+1]/q);
      r = abs(p[j+2]/q);

      p[j] = b;
      p[j+1] = g;
      p[j+2] = r;
    }
  }
return frame;
}

Mat& dif(Mat& frame) {
  int channels = frame.channels();

  int nRows = frame.rows;
  int nCols = frame.cols * channels;
  unsigned char y;

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
      y = p[j]+128%255;

      p[j] = y;
      p[j+1] = y;
      p[j+2] = y;
    }
  }

return frame;
}
int main(int argc, char* argv[])
{

  if (argc<4) {
    cout << "not enough arguments supplied!" << endl;
    cout << "use: ./dif video_file_to_read.avi video_file_to_save 0-64" << endl;
    return -1;
  }

  int alpha = 0;
  if(atof(argv[3])>=0 && atof(argv[3])<=64) 
  {
      alpha = atof(argv[3]); 
  }
  else 
  {
      cout << "ERROR: q must be > 0 && <= 64";
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

    Mat dif_mat = frame - prev_frame;
    dif(dif_mat);
    imshow("differenzbild", dif_mat);
    //oVideoWriter.write(dif);

    Mat dif_red = reduce(dif_mat, alpha);
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
