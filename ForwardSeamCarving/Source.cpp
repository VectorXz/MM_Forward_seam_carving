#include<stdio.h>
#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/shape/shape.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<stdlib.h>
#include<iostream>
#include<conio.h>

using namespace cv;
using namespace std;

//Foreard Seam Carving
int main()
{
	//Display an Image
	Mat img = imread("img_lights.jpg", IMREAD_COLOR);
	namedWindow("Example", WINDOW_AUTOSIZE);
	imshow("Example", img);
	int c = cvWaitKey(0);
	Size sz = img.size();
	int height = sz.height;
	int width = sz.width;

	//Esc = 27, a = 97, d = 100, s = 115, w = 119
	while (c != 27) {
		Mat grayimg;
		Mat grayimg_border;
		cvtColor(img, grayimg, COLOR_BGR2GRAY);
		// Defining M matrix -> to keep energy
		// Defining K matrix -> to keep best seam
		Mat M = (Mat_<int>(5, 7));
		Mat K = (Mat_<int>(5, 7));

		//Mat imgwpad(height + 2, width + 2, CV_8UC1, Scalar(0));
		Mat original = (Mat_<int>(3, 5) << 14, 24, 35, 13, 100, 36, 18, 11, 20, 60, 21, 19, 72, 120, 14);
		cout << "Width" << width << "Height" << height << "\n";
		cout << "Width" << original.cols << "Height" << original.rows << endl;

		int border = 1;
		copyMakeBorder(original, grayimg_border, border, border, border, border, BORDER_REPLICATE);
		cout << grayimg_border << endl;

		//Looping till get the command 'a', 'd', 'w', 's'
		while (c != 97 && c != 100 && c != 119 && c != 27) {
			c = cvWaitKey(0);
		}

		//Keyboard command ::: 'a' and 'd' => vertical best seam, 'w' and 's' => horizontal best seam
		//'a' => Reduce width, 'd' => increase width
		if (c == 97 || c == 100) {
			//Construct M matrix and K matrix in vertical direction
			// *** WRITE YOUR CODE ***
			
			//CALCULATE M MATRIX
			int x, y;
			for (x = 0; x < grayimg_border.rows; x++)
			{
				for (y = 0; y < grayimg_border.cols; y++) {
					//cout << x << "|" << y << " ";
					if (x > 0 && x < grayimg_border.rows-1 && y > 0 && y < grayimg_border.cols-1) {
						//cout << grayimg_border.at<int>(Point(y, x)) << " ";
						//calculate Top Left
						int cl = abs(grayimg_border.at<int>(x, y + 1) - grayimg_border.at<int>(x, y - 1)) + abs(grayimg_border.at<int>(x - 1, y) - grayimg_border.at<int>(x, y - 1));
						int cu = abs(grayimg_border.at<int>(x, y + 1) - grayimg_border.at<int>(x, y - 1));
						int cr = abs(grayimg_border.at<int>(x, y + 1) - grayimg_border.at<int>(x, y - 1)) + abs(grayimg_border.at<int>(x - 1, y) - grayimg_border.at<int>(x, y + 1));
						int tempM[3],min=999;
						tempM[0] = M.at<int>(x - 1, y - 1) + cl;
						tempM[1] = M.at<int>(x - 1, y) + cu;
						tempM[2] = M.at<int>(x - 1, y + 1) + cr;
						for (int i = 0; i < 3; i++) {
							if (tempM[i] < min) { min = tempM[i]; }
						}
						M.at<int>(x, y) = min;
					}
					else
					{
						M.at<int>(x, y) = 0;
					}
				}
				cout << endl;
			}

			cout << M << endl;

			//Find the best seam in the vertical direction
			// *** WRITE YOUR CODE ***
		}

		//'w' => Increase height, 's' => reduce height
		if (c == 115 || c == 119) {
			//Construct M matrix and K matrix in horizontal direction
			// *** WRITE YOUR CODE ***

			//Find the best seam in the horizontal direction
			// *** WRITE YOUR CODE ***
		}

		//Insert or delete the best seam
		if (c == 97) {
			//Reduce width or delete seam verticaly
			//Copy the pixels into this image
			Mat img_new(height, --width, CV_64FC3, Scalar(0, 0, 0));
			// *** WRITE YOUR CODE ***

			//Show the resized image
			imshow("Example", img_new);
			//Clone img_new into ing for the nest loop processing
			img.release();
			//img = img_new.clone;
			//img_new.release();
		}
		if (c == 100) {
			//Increase width or insert seam vertically
			//Copy the pixels into this image
			Mat img_new(height, ++width, CV_64FC3, Scalar(0, 0, 0));
			// *** WRITE YOUR CODE ***

			//Show the resized image
			imshow("Example", img_new);
			//Clone img_new into img for the next loop processing
			img.release();
			img = img_new.clone();
			img_new.release();
		}
		if (c == 115) {
			//Reduce height or delete seam horizontally
			//Copy the pixels into this image
			Mat img_new(--height, width, CV_64FC3, Scalar(0, 0, 0));
			// *** WRITE YOUR CODE ***

			//Show the resized image
			imshow("Example", img_new);
			//Clone img_new into img for the next loop processing
			img.release();
			img = img_new.clone();
			img_new.release();
		}
		if (c == 119) {
			//Increase height or insert seam horizontally
			//Copy the pixels into this image
			Mat img_new(++height, width, CV_64FC3, Scalar(0, 0, 0));
			// *** WRITE YOUR CODE ***

			//Show the resized image
			imshow("Example", img_new);
			//Clone img_new into img for the next loop processing
			img.release();
			img = img_new.clone();
			img_new.release();
		}
		if (c == 27) {
			break;
		}
		c = cvWaitKey(0);
	}
	return 0;
}