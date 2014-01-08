#include <iostream>

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;
using namespace std;

void color_reduce(Mat &input, Mat &output, size_t div)
{
  if(div==0) 
  {
    output = input;
    return;
  }
  if(input.data != output.data)
  {
    output.create(input.size(), input.type());
  }

  uchar buffer[256];
  for(size_t i=0; i!=256; ++i)
  {
    buffer[i] = i / div * div + div / 2;
  }
  Mat table(1, 256, CV_8U, buffer, sizeof(buffer));
  LUT(input, table, output);
}

int main(int argc, char* argv[])
{
  if(argc<3) 
  {
    cout<<"Not enough arguments supplied!"<<endl;
    return -1;
  }

  VideoCapture cap(argv[1]);

  if(!cap.isOpened())
  {
  cout<<"Cannot open the video file"<<endl;
  return -1;
  }

  double dWidth = cap.get(CV_CAP_PROP_FRAME_WIDTH);
  double dHeight = cap.get(CV_CAP_PROP_FRAME_HEIGHT);

  cout<<"Frame Size = "<<dWidth<<"x"<<dHeight<<endl;

  Size frameSize(static_cast<int>(dWidth), static_cast<int>(dHeight));
  /*
  VideoWriter oVideoWriter (argv[2], CV_FOURCC('P','I','M','1'), 20, frameSize, true); 

  if(!oVideoWriter.isOpened())
  {
    cout<<"ERROR: Failed to write the video"<<endl;
    return -1;
  }
  */
  while(1)
  {
    Mat frame;
    Mat output;
    bool bSuccess = cap.read(frame);

    if(!bSuccess)
    {
      cout<<"ERROR: Cannot read a frame from video file"<<endl;
      break;
    }
    
    color_reduce(frame, output, strtol(argv[2], NULL, 10));
    imshow("src", frame);
    imshow("quantized", output);
    
    if (waitKey(10) == 27)
    {
      cout<<"esc key is pressed by user"<<endl;
      break; 
    }
  }
  return 0;
}
