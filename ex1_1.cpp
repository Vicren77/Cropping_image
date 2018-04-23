//edited by Ganbayar
//
//get coordinate of image and save it as text file
//

#include "stdafx.h"
#include<opencv\highgui.h>
#include<opencv\cv.h>
#include <iostream>
#include <atlstr.h>
#include <fstream>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>

using namespace std;
using namespace cv;

 struct mouse_info
{
 int x;
 int y;
}upper,lower;

  struct image
{
 IplImage* orig;
 IplImage* cropped;
 IplImage* test;
}myimg;


void CallBackFunc(int event, int x, int y, int flags, void* userdata)
{

	cvSetZero(myimg.test);//delete test img
	cvCopy(myimg.orig,myimg.test);

     if  ( event == EVENT_LBUTTONDOWN )
     {
         // cout << "Left button of the mouse is clicked - position (" << x << ", " << y << ")" << endl;
		  upper.x=x;
		  upper.y=y;
     }
	  else if  ( event == EVENT_LBUTTONUP )
     {
          //cout << "Left button of the mouse is released - position (" << x << ", " << y << ")" << endl;
		  lower.x=x;
		  lower.y=y;
		  cvRectangle(myimg.test, cvPoint(upper.x, upper.y), cvPoint(lower.x, lower.y), CV_RGB(0,0,255),1);
		  cvShowImage("My Window",myimg.test);
     }
}


int _tmain(int argc, _TCHAR* argv[])
{
	
	TCHAR szDir[MAX_PATH];
	HANDLE hFind = INVALID_HANDLE_VALUE;
	WIN32_FIND_DATA ffd;
	CString Path_Original = "F:\\DBs\\2016-12-09 CNN DB\\people\\8\\backside\\4\\";//frontside backside
// CString Path_Original=".\\DB\\";//DB location

   hFind = FindFirstFile(Path_Original+"*.bmp", &ffd);
   //IplImage* cropped_img;
   IplImage* orig_img;


   //generate new folder
   CString t_DR_RPath6;
   t_DR_RPath6 = Path_Original + "\\t\\";
   CreateDirectoryA(t_DR_RPath6, NULL);

	int last_frame=3950;//iteration number defined by number of image in the DB
		for (int f=0;f<last_frame; f++)
{

	CString file_name=ffd.cFileName;
	CString Final_Thermal_Path = Path_Original +file_name;
	orig_img=cvLoadImage(Final_Thermal_Path,-1);
	FindNextFile(hFind, &ffd);
		printf(file_name);
		printf("\n");
	CvSize dim=cvGetSize(orig_img);

	//myimg.orig=cvCreateImage(cvSize(dim.width, dim.height), 8, 3);//initialize orig img for visible image
	myimg.orig = cvCreateImage(cvSize(dim.width, dim.height), 8, 1);//initialize orig img for thermal image
	cvSetZero(myimg.orig);
	cvCopy(orig_img,myimg.orig);//copy orig img

	//myimg.test=cvCreateImage(cvSize(dim.width, dim.height), 8, 3);//initialize test image for visible image
	myimg.test = cvCreateImage(cvSize(dim.width, dim.height), 8, 1);//initialize test image for thermal image
	cvSetZero(myimg.test);//blank

	cvShowImage("My Window",myimg.orig);
	namedWindow("My Window", 1);

	GRASSFIRE:
	setMouseCallback("My Window", CallBackFunc, NULL);

		cvWaitKey(0);//after press button save cropped image

		if (upper.x < 0)//to fix minus number
			upper.x = 0;
		else if (upper.x >dim.width)
				 upper.x = dim.width;
		if (upper.y < 0)
			upper.y = 0;
			else if (upper.y >dim.height)
				     upper.y = dim.height;
		if (lower.x < 0)
			lower.x = 0;
		else if (lower.x >dim.width)
			lower.x = dim.width;
		if (lower.y < 0)
			lower.y = 0;
		else if (lower.y >dim.height)
			lower.y = dim.height;

		CvRect rec;
		if(upper.x<lower.x && upper.y<lower.y)//draw rectangle starting from any point
		{
				
				rec.x = upper.x;
				rec.y= upper.y;
				rec.width = lower.x-upper.x;
				rec.height =lower.y-upper.y;
		}
		else if(upper.x>lower.x && upper.y<lower.y)//draw rectangle starting from any point
		{
				rec.x = lower.x;
				rec.y= upper.y;
				rec.width = upper.x-lower.x;
				rec.height =lower.y-upper.y;
		}
		else if(upper.x<lower.x && upper.y>lower.y)//draw rectangle starting from any point
		{
				rec.x = upper.x;
				rec.y= lower.y;
				rec.width = lower.x-upper.x;
				rec.height =upper.y-lower.y;
		}
		else if(upper.x>lower.x && upper.y>lower.y)//draw rectangle starting from any point
		{
				rec.x = lower.x;
				rec.y= lower.y;
				rec.width = upper.x-lower.x;
				rec.height =upper.y-lower.y;
		}
		else 
			printf("ERROR to fix");

		//justification for division factor of 4*********************************************************
		int w = rec.width;
		int r = w % 4;

		if (r == 1)
		{
			w = w + 3;
		}

		else if (r == 2)
		{
			w = w + 2;
		}

		else if (r == 3)
		{
			w = w + 1;
		}

		CString pname = Path_Original + "\\t\\coordinate.txt";	
		FILE * coord = fopen(pname, "a+");
		    fprintf(coord, "%s\n", file_name);
			fprintf(coord, "%d,%d,%d,%d\n", rec.x,rec.y,w,rec.height);
		fclose(coord);					
				cvResetImageROI(orig_img);
}
    cvWaitKey(0);
return 0;
}




//
//
//void CallBackFunc(int event, int x, int y, int flags, void* userdata)
//{
//     if  ( event == EVENT_LBUTTONDOWN )
//     {
//          cout << "Left button of the mouse is clicked - position (" << x << ", " << y << ")" << endl;
//     }
//	  else if  ( event == EVENT_LBUTTONUP )
//     {
//          cout << "Left button of the mouse is released - position (" << x << ", " << y << ")" << endl;
//     }
//     else if  ( event == EVENT_RBUTTONDOWN )
//     {
//          cout << "Right button of the mouse is clicked - position (" << x << ", " << y << ")" << endl;
//     }
//	  else if  ( event == EVENT_RBUTTONUP )
//     {
//          cout << "Right button of the mouse is released - position (" << x << ", " << y << ")" << endl;
//     }
//     else if  ( event == EVENT_MBUTTONDOWN )
//     {
//          cout << "Middle button of the mouse is clicked - position (" << x << ", " << y << ")" << endl;
//     }
//	  else if  ( event == EVENT_MBUTTONUP )
//     {
//          cout << "Middle button of the mouse is released - position (" << x << ", " << y << ")" << endl;
//     }
//	 //double clicked
//	 /*EVENT_LBUTTONDBLCLK
//	 EVENT_RBUTTONDBLCLK
//	 EVENT_MBUTTONDBLCLK*/
//     else if ( event == EVENT_MOUSEMOVE )
//     {
//          cout << "Mouse move over the window - position (" << x << ", " << y << ")" << endl;
//
//     }
//}


//void CallBackFunc(int event, int x, int y, int flags, void* userdata)
//{
//     if ( flags == (EVENT_FLAG_CTRLKEY + EVENT_FLAG_LBUTTON) )
//     {
//          cout << "Left mouse button is clicked while pressing CTRL key - position (" << x << ", " << y << ")" << endl;
//     }
//     else if ( flags == (EVENT_FLAG_RBUTTON + EVENT_FLAG_SHIFTKEY) )
//     {
//          cout << "Right mouse button is clicked while pressing SHIFT key - position (" << x << ", " << y << ")" << endl;
//     }
//     else if ( event == EVENT_MOUSEMOVE && flags == EVENT_FLAG_ALTKEY)
//     {
//          cout << "Mouse is moved over the window while pressing ALT key - position (" << x << ", " << y << ")" << endl;
//     }
//}



//EVENT_FLAG_LBUTTON
//EVENT_FLAG_RBUTTON
//EVENT_FLAG_MBUTTON
//EVENT_FLAG_CTRLKEY
//EVENT_FLAG_SHIFTKEY
//EVENT_FLAG_ALTKEY
