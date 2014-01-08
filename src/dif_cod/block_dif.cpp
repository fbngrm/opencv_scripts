#include "opencv2/highgui/highgui.hpp"
#include <iostream>

using namespace cv;
using namespace std;

// compile:
// g++ -o open_video open_video.cpp `pkg-config opencv --cflags --libs`

// calculate color values for pixel-matrices in contiuous frames
// @param frame: the Mat to work with
// @param size: dimension of the matrices = size*size
// @param width: the width of the frame
Mat& continuousReduce(Mat& frame, int size, int width)
{
  // if matrix dimension is 1 - return the original frame
  if(size < 1) 
  {
    return frame;
  }

  // get some frame data
  int channels = frame.channels();
  int rows = frame.rows;
  int cols = frame.cols * channels;

  cols *= rows;
  rows = 1;

  // check if the given matrix size is valid
  if((cols / channels) % size != 0)
  {
    cout << "ERROR: can not calculate matrix with th given size";
    cout << "rows: " << rows << endl;
    cout << "columns: " << cols << endl;
    cout << "size: " << size << endl;

    return frame;
  }

  // declare variables for iteration and color calculation
  unsigned int i,j,k,l,n,o;
  unsigned int r_sum, g_sum, b_sum = 0;

  // pointers for block the row
  uchar* p = frame.ptr<uchar>(0);

  // block-rows
  for(i=0; i<cols; i+=size*width*channels)
  {
    // block-columns
    for(j=0; j<width*channels; j+=size*channels)
    {
      // pixel-rows
      for(k=0; k<size*width*channels; k+=width*channels)
      {
        // pixel
        for(l=0; l<size*channels; l+=channels)
        {
        /*
        cout << "foo[" << i+j+k+l   << "] = " << int(p[i+j+k+l])   << endl; 
        cout << "foo[" << i+j+k+l+1 << "] = " << int(p[i+j+k+l+1]) << endl; 
        cout << "foo[" << i+j+k+l+2 << "] = " << int(p[i+j+k+l+2]) << endl; 
        */
        // sum up all bgr-values of all pixels in the matrix
        b_sum += (unsigned int)p[i+j+k+l];
        g_sum += (unsigned int)p[i+j+k+l+1];
        r_sum += (unsigned int)p[i+j+k+l+2]; 
        }
      }

      // get the average of the color sum in the block
      b_sum = (unsigned char)(b_sum/(size*size));
      g_sum = (unsigned char)(g_sum/(size*size));
      r_sum = (unsigned char)(r_sum/(size*size));

      // write averaged color value to all pixels in the block
      for(n=0; n<size*width*channels; n+=width*channels)
      {
        for(o=0; o<size*channels; o+=channels)
        {
        /*
        cout << "foo[" << i+j+n+o   << "] = " << b_sum << endl; 
        cout << "foo[" << i+j+n+o+1 << "] = " << g_sum << endl; 
        cout << "foo[" << i+j+n+o+2 << "] = " << r_sum << endl; 
        */
        p[i+j+n+o]   = b_sum;
        p[i+j+n+o+1] = g_sum;
        p[i+j+n+o+2] = r_sum;
        }
      }
      // reset counters for bgr-values of the pixel-blocks
      b_sum = 0;
      g_sum = 0;
      r_sum = 0;
    }
  }
  return frame;
}


Mat& blockReduce(Mat& frame, int size, int width) 
{
  // check if the given frame is countinuous
  // see: http://docs.opencv.org/doc/tutorials/core/how_to_scan_images/how_to_scan_images.html#howtoscanimagesopencv
  // for more information on how to scan images in opencv
  if (frame.isContinuous())
  {
    return continuousReduce(frame, size, width);
  } 
  else
  {
    return frame;
  }
}

// divide exery pixel in the frame with param q
// FIX ME: use look-up-table to increase the speed
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
  for(i=0; i<nRows; ++i)
  {
    p = frame.ptr<uchar>(i);
    for(j=0; j<nCols; j+=3)
    {
      b = p[j]/q;
      g = p[j+1]/q;
      r = p[j+2]/q;

      p[j]   = b;
      p[j+1] = g;
      p[j+2] = r;
    }
  }
  return frame;
}

int main(int argc, char* argv[])
{

  if(argc<3) 
  {
    cout << "not enough arguments supplied!" << endl;
    cout << "use: ./block_dif read_video_from.avi write_video_to.avi" << endl;
    return -1;
  }

  VideoCapture cap(argv[1]);

  if (!cap.isOpened())
  {
    cout << "cannot open the video file" << endl;
    return -1;
  }

  // get the dimensions of the video
  double dWidth = cap.get(CV_CAP_PROP_FRAME_WIDTH);
  double dHeight = cap.get(CV_CAP_PROP_FRAME_HEIGHT);

  cout << "frame Size = " << dWidth << "x" << dHeight << endl;
  Size frameSize(static_cast<int>(dWidth), static_cast<int>(dHeight));

  // initialize video-writer object
  VideoWriter oVideoWriter (argv[2], CV_FOURCC('P','I','M','1'), 20, frameSize, true); 

  if(!oVideoWriter.isOpened())
  {
    cout << "ERROR: Failed to write the video" << endl;
    return -1;
  }

  Mat prev_frame;
  cap.read(prev_frame);

  while (1)
  {
    Mat cur_frame;
    bool bSuccess = cap.read(cur_frame);

    if (!bSuccess)
    {
      cout << "ERROR: Cannot read a frame from video file" << endl;
      break;
    }

    imshow("original", cur_frame);

    Mat cur_frame_tmp = cur_frame.clone();
    blockReduce(cur_frame, 4, dWidth);
    blockReduce(prev_frame, 4, dWidth);
    Mat dif_frame = cur_frame - prev_frame;
    
    imshow("differenzbild", dif_frame);
    imshow("blocks", cur_frame);

    reduce(dif_frame, 2);
    imshow("quantisiertes differenzbild", dif_frame);

    prev_frame = cur_frame_tmp;

    oVideoWriter.write(cur_frame);


    if (waitKey(10) == 27)
    {
      cout << "esc key is pressed by user" << endl;
      break; 
    }
  }
  return 0;
}

/*
void testPixelOrder()
{
int channels = 3;
int foo[108];

for(int i=0; i<108; i+=3)
{
foo[i]   = 'b';
foo[i+1] = 'g';
foo[i+2] = 'r';

//cout << "foo[" << i <<"]: " << foo[i] << endl;
}

int size  = 3;
int width = 6;
int cols  = 36*channels;

int i,j,k,l;

for(i=0; i<cols; i+=size*width*channels)
{
for(j=0; j<width*channels; j+=size*channels)
{
for(k=0; k<size*width*channels; k+=width*channels)
{
for(l=0; l<size*channels; l+=channels)
{
//cout << "i: " << i << endl;
//cout << "j: " << j << endl;
//cout << "k: " << k << endl;
//cout << "l: " << l << endl;
cout << "foo[" << i+j+k+l   << "] = " << char(foo[i+j+k+l])   << endl; 
cout << "foo[" << i+j+k+l+1 << "] = " << char(foo[i+j+k+l+1]) << endl; 
cout << "foo[" << i+j+k+l+2 << "] = " << char(foo[i+j+k+l+2]) << endl; 

}
}
cout << "---" << endl;
}
}

}
*/
