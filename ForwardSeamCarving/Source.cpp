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

//Forward Seam Carving
int main()
{
	//Display an Image
	Mat img = imread("fw.jpg", IMREAD_COLOR);
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
		Mat tCol;
		Mat tRow;
		cvtColor(img, grayimg, COLOR_BGR2GRAY);
		// Defining M matrix -> to keep energy
		// Defining K matrix -> to keep best seam
		Mat M = (Mat_<int>(height, width));
		Mat K = (Mat_<int>(height, width));

		// PADDING WITH DUPLICATE START //
		int border = 1;
		copyMakeBorder(grayimg, grayimg_border, border, border, border, border, BORDER_REPLICATE);
		//PADDING WITH DUPLICATE END //

		//Looping till get the command 'a', 'd', 'w', 's'
		while (c != 97 && c != 100 && c != 119 && c != 115) {
			c = cvWaitKey(0);
		}

		//Keyboard command ::: 'a' and 'd' => vertical best seam, 'w' and 's' => horizontal best seam
		//'a' => Reduce width, 'd' => increase width
		if (c == 97 || c == 100) {
			//Construct M matrix and K matrix in vertical direction
			// *** WRITE YOUR CODE ***
			//CALCULATE M,K MATRIX
			int x, y;
			for (x = 0; x < grayimg_border.rows - 2; x++)
			{
				for (y = 0; y < grayimg_border.cols - 2; y++) {
					//if it's the first row > just calculate difference of left-right
					if (x == 0) {
						M.at<int>(x, y) = abs((int)grayimg_border.at<uchar>(x + 1, y) - (int)grayimg_border.at<uchar>(x + 1, y + 2));
						// all elements in first row of K matrix are 0
						K.at<int>(x, y) = 0;
					}
					//else just calculate cl,cu,cr
					else {
						int cl = abs((int)grayimg_border.at<uchar>(x, y + 1) - (int)grayimg_border.at<uchar>(x + 1, y)) + abs((int)grayimg_border.at<uchar>(x + 1, y + 2) - (int)grayimg_border.at<uchar>(x + 1, y));
						int cu = abs((int)grayimg_border.at<uchar>(x, y + 1) - (int)grayimg_border.at<uchar>(x + 1, y));
						int cr = abs((int)grayimg_border.at<uchar>(x, y + 2) - (int)grayimg_border.at<uchar>(x + 1, y)) + abs((int)grayimg_border.at<uchar>(x + 1, y + 2) - (int)grayimg_border.at<uchar>(x + 1, y));
						int min;
						//if y == 0 > first col > no M @ CL > not considering CL to find min
						if (y == 0) {
							min = std::min({ M.at<int>(x - 1, y) + cu, M.at<int>(x - 1, y + 1) + cr });
							if (min == (M.at<int>(x - 1, y) + cu)) {
								K.at<int>(x, y) = 2;
							}
							else if (min == (M.at<int>(x - 1, y + 1) + cr)) {
								K.at<int>(x, y) = 3;
							}
						}
						//if y == last col > no M @ CR > not considering CR to find min
						else if (y == grayimg_border.cols - 3) {
							min = std::min({ M.at<int>(x - 1, y - 1) + cl, M.at<int>(x - 1, y) + cu });
							if (min == (M.at<int>(x - 1, y - 1) + cl)) {
								K.at<int>(x, y) = 1;
							}
							else if (min == (M.at<int>(x - 1, y) + cu)) {
								K.at<int>(x, y) = 2;
							}
						}
						//else just consider 3 things to find min
						else {
							min = std::min({ M.at<int>(x - 1, y - 1) + cl, M.at<int>(x - 1, y) + cu, M.at<int>(x - 1, y + 1) + cr });
							if (min == (M.at<int>(x - 1, y - 1) + cl)) {
								//if min = cl then k = 1
								K.at<int>(x, y) = 1;
							}
							else if (min == (M.at<int>(x - 1, y) + cu)) {
								//if min = cu then k = 2
								K.at<int>(x, y) = 2;
							}
							else if (min == (M.at<int>(x - 1, y + 1) + cr)) {
								//if min = cr then k = 3
								K.at<int>(x, y) = 3;
							}
						}
						M.at<int>(x, y) = min;
					}
				}
			}

			//Find the best seam in the vertical direction
			// *** WRITE YOUR CODE ***
			//declare tCol matrix to keep seam index in each col
			tCol = (Mat_<int>(1, height));
			//set min value to the first member
			int min = M.at<int>(height - 1, 0);
			int heightCount = height - 1;
			tCol.at<int>(0, heightCount) = 0;
			//looping to find min in last row
			for (y = 0; y < width; y++) {
				if (M.at<int>(height - 1, y) < min) {
					min = M.at<int>(height - 1, y);
					tCol.at<int>(0, heightCount) = y;
				}
			}

			//after get min index then continue looping to find next row index of best seam based on value in K
			while (heightCount != 0) {
				//if K = 1 then next row will take left value means (index-1)
				if (K.at<int>(heightCount, tCol.at<int>(0, heightCount)) == 1) {
					tCol.at<int>(0, heightCount - 1) = tCol.at<int>(0, heightCount) - 1;
				}
				//if K = 2 then next row will take up value (remain same index)
				else if (K.at<int>(heightCount, tCol.at<int>(0, heightCount)) == 2) {
					tCol.at<int>(0, heightCount - 1) = tCol.at<int>(0, heightCount);
				}
				//if K = 3 then next row will take right value means (index+1)
				else if (K.at<int>(heightCount, tCol.at<int>(0, heightCount)) == 3) {
					tCol.at<int>(0, heightCount - 1) = tCol.at<int>(0, heightCount) + 1;
				}
				heightCount--;
			}
		}

		//'w' => Increase height, 's' => reduce height
		if (c == 115 || c == 119) {
			//CALCULATE M,K MATRIX
			int x, y;
			//we loop at column first then row
			for (y = 0; y < grayimg_border.cols - 2; y++)
			{
				for (x = 0; x < grayimg_border.rows - 2; x++) {
					//if it's the first column > just calculate difference of up/down
					if (y == 0) {
						M.at<int>(x, y) = abs((int)grayimg_border.at<uchar>(x, y + 1) - (int)grayimg_border.at<uchar>(x + 2, y + 1));
						// all elements in first col of K matrix are 0
						K.at<int>(x, y) = 0;
					}
					//else just calculate cl,cu,cr
					else {
						int cl = abs((int)grayimg_border.at<uchar>(x + 1, y) - (int)grayimg_border.at<uchar>(x + 2, y + 1)) + abs((int)grayimg_border.at<uchar>(x, y + 1) - (int)grayimg_border.at<uchar>(x + 2, y + 1));
						int cu = abs((int)grayimg_border.at<uchar>(x, y + 1) - (int)grayimg_border.at<uchar>(x + 2, y + 1));
						int cr = abs((int)grayimg_border.at<uchar>(x + 1, y) - (int)grayimg_border.at<uchar>(x, y + 1)) + abs((int)grayimg_border.at<uchar>(x, y + 1) - (int)grayimg_border.at<uchar>(x + 2, y + 1));
						int min;
						//if x == last row > no M @ CL > not considering CL to find min
						if (x == height - 1) {
							min = std::min({ M.at<int>(x, y - 1) + cu, M.at<int>(x - 1, y - 1) + cr });
							if (min == (M.at<int>(x, y - 1) + cu)) {
								K.at<int>(x, y) = 2;
							}
							else if (min == (M.at<int>(x - 1, y - 1) + cr)) {
								K.at<int>(x, y) = 3;
							}
						}
						//if x == fist col > no M @ CR > not considering CR to find min
						else if (x == 0) {
							min = std::min({ M.at<int>(x + 1, y - 1) + cl, M.at<int>(x, y - 1) + cu });
							if (min == (M.at<int>(x + 1, y - 1) + cl)) {
								K.at<int>(x, y) = 1;
							}
							else if (min == (M.at<int>(x, y - 1) + cu)) {
								K.at<int>(x, y) = 2;
							}
						}
						//else just consider 3 things to find min
						else {
							min = std::min({ M.at<int>(x + 1, y - 1) + cl, M.at<int>(x, y - 1) + cu, M.at<int>(x - 1, y - 1) + cr });
							if (min == (M.at<int>(x + 1, y - 1) + cl)) {
								//if min = cl > k = 1
								K.at<int>(x, y) = 1;
							}
							else if (min == (M.at<int>(x, y - 1) + cu)) {
								//if min = cu > k = 2
								K.at<int>(x, y) = 2;
							}
							else if (min == (M.at<int>(x - 1, y - 1) + cr)) {
								//if min = cr > k = 3
								K.at<int>(x, y) = 3;
							}
						}
						M.at<int>(x, y) = min;
					}
				}
			}

			//Find the best seam in the horizontal direction
			// *** WRITE YOUR CODE ***
			//declare tRow to keep best seam index in each row
			tRow = (Mat_<int>(1, width));
			//initialize the first value
			int min = M.at<int>(0, width - 1);
			int widthCount = width - 1;
			tRow.at<int>(0, widthCount) = 0;
			//looping to find min
			for (x = 0; x < height; x++) {
				if (M.at<int>(x, width - 1) <= min) {
					min = M.at<int>(x, width - 1);
					tRow.at<int>(0, widthCount) = x;
				}
			}

			//continue to find best seam index in each colum
			while (widthCount != 0) {
				//if K == 1 > take left index > index + 1 (lower row member)
				if (K.at<int>(tRow.at<int>(0, widthCount), widthCount) == 1) {
					tRow.at<int>(0, widthCount - 1) = tRow.at<int>(0, widthCount) + 1;
				}
				//if K == 2 > take same index
				else if (K.at<int>(tRow.at<int>(0, widthCount), widthCount) == 2) {
					tRow.at<int>(0, widthCount - 1) = tRow.at<int>(0, widthCount);
				}
				//if K == 3 > take right index > index - 2 (higher row number)
				else if (K.at<int>(tRow.at<int>(0, widthCount), widthCount) == 3) {
					tRow.at<int>(0, widthCount - 1) = tRow.at<int>(0, widthCount) - 1;
				}
				widthCount--;
			}
		}

		//Insert or delete the best seam
		if (c == 97) {
			//Reduce width or delete seam verticaly
			//Copy the pixels into this image
			Mat img_new(height, --width, CV_8UC3, Scalar(0, 0, 0));
			// *** WRITE YOUR CODE ***
			int i, j;
			//initialize shift to be false
			bool shift = false;
			//looping for construct new image
			for (i = 0; i < height; i++) {
				//get seam index that we kept in tCol
				int seamIndex = tCol.at<int>(0, i);
				//reset shifting in each row
				shift = false;
				for (j = 0; j <= width; j++) {
					if (j == seamIndex) {
						//if j == seam index then make shift to true and skip this pixels
						shift = true;
						continue;
					}
					//if shift is true > put current pixel into before pixels (j-1) that we skipped
					if (shift == true) {
						img_new.at<Vec3b>(i, j - 1) = img.at<Vec3b>(i, j);
					}
					else
					//else just copy pixel
					{
						img_new.at<Vec3b>(i, j) = img.at<Vec3b>(i, j);
					}
				}
			}

			//Show the resized image
			imshow("Example", img_new);
			//Clone img_new into iทg for the nest loop processing
			img.release();
			img = img_new.clone();
			img_new.release();
		}
		if (c == 100) {
			//Increase width or insert seam vertically
			//Copy the pixels into this image
			Mat img_new(height, ++width, CV_8UC3, Scalar(0, 0, 0));
			// *** WRITE YOUR CODE ***
			int i, j;
			//initialize shift to be false
			bool shift = false;
			//loop for constructing image
			for (i = 0; i < height; i++) {
				//get seam index
				int seamIndex = tCol.at<int>(0, i);
				shift = false;
				for (j = 0; j < width; j++) {
					//if j == seamindex then we copy current pixel and set shift to true
					if (j == seamIndex) {
						shift = true;
						//printf("%d,%d ", i, j);
						img_new.at<Vec3b>(i, j) = img.at<Vec3b>(i, j);
						continue;
					}
					//if shift is true so we copy before pixels to current pixel
					if (shift == true) {
						img_new.at<Vec3b>(i, j) = img.at<Vec3b>(i, j - 1);
					}
					else
					{
					//else just copy it
						img_new.at<Vec3b>(i, j) = img.at<Vec3b>(i, j);
					}
				}
			}

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
			Mat img_new(--height, width, CV_8UC3, Scalar(0, 0, 0));
			// *** WRITE YOUR CODE ***
			int i, j;
			//initialize shift to be false
			bool shift = false;
			//loop for constructing image
			for (i = 0; i < width; i++) {
				//get seam index from tRow
				int seamIndex = tRow.at<int>(0, i);
				//reset shift in each round
				shift = false;
				for (j = 0; j < height; j++) {
					//if j = seam index then set shift to be true
					if (j == seamIndex) {
						shift = true;
					}
					if (shift == true) {
					//if shift = true then we copy next image to current image
						img_new.at<Vec3b>(j, i) = img.at<Vec3b>(j + 1, i);
					}
					else {
					//else just copy
						img_new.at<Vec3b>(j, i) = img.at<Vec3b>(j, i);
					}
				}
			}

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
			Mat img_new(++height, width, CV_8UC3, Scalar(0, 0, 0));
			// *** WRITE YOUR CODE ***
			int i, j;
			//initialize shift to false
			bool shift = false;
			//loop for constructing image
			for (i = 0; i < width; i++) {
				//get seam index
				int seamIndex = tRow.at<int>(0, i);
				//reset shift to be false in each round
				shift = false;
				for (j = 0; j < height; j++) {
					//if j = seamindex > set shift to true and copy current pixels
					if (j == seamIndex) {
						shift = true;
						img_new.at<Vec3b>(j, i) = img.at<Vec3b>(j, i);
						continue;
					}
					//if shift = true then copy last pixel to current pixel
					if (shift == true) {
						img_new.at<Vec3b>(j, i) = img.at<Vec3b>(j - 1, i);
					}
					else {
					//else just copy it
						img_new.at<Vec3b>(j, i) = img.at<Vec3b>(j, i);
					}
				}
			}

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