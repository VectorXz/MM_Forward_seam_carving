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
		//cout << grayimg << endl;

		//Looping till get the command 'a', 'd', 'w', 's'
		while (c != 97 && c != 100 && c != 119 && c != 115) {
			c = cvWaitKey(0);
		}

		//Keyboard command ::: 'a' and 'd' => vertical best seam, 'w' and 's' => horizontal best seam
		//'a' => Reduce width, 'd' => increase width
		if (c == 97 || c == 100) {
			//Construct M matrix and K matrix in vertical direction
			// *** WRITE YOUR CODE ***
			//CALCULATE M MATRIX
			int x, y;
			for (x = 0; x < grayimg_border.rows - 2; x++)
			{
				for (y = 0; y < grayimg_border.cols - 2; y++) {
					//cout << x << "|" << y << " ";
					//if it's the first row > just calculate difference of left/right
					if (x == 0) {
						M.at<int>(x, y) = abs((int)grayimg_border.at<uchar>(x + 1, y) - (int)grayimg_border.at<uchar>(x + 1, y + 2));
						// all elements in first row of K matrix are 0
						K.at<int>(x, y) = 0;
						//cout << M.at<int>(x, y) << " ";
					}
					//else just calculate cl,cu,cr
					else {
						int cl = abs((int)grayimg_border.at<uchar>(x, y + 1) - (int)grayimg_border.at<uchar>(x + 1, y)) + abs((int)grayimg_border.at<uchar>(x + 1, y + 2) - (int)grayimg_border.at<uchar>(x + 1, y));
						int cu = abs((int)grayimg_border.at<uchar>(x, y + 1) - (int)grayimg_border.at<uchar>(x + 1, y));
						int cr = abs((int)grayimg_border.at<uchar>(x, y + 2) - (int)grayimg_border.at<uchar>(x + 1, y)) + abs((int)grayimg_border.at<uchar>(x + 1, y + 2) - (int)grayimg_border.at<uchar>(x + 1, y));
						int min;
						//if y == 0 > no M @ CL > not considering CL to find min
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
								//if min = cl > k = 1
								K.at<int>(x, y) = 1;
							}
							else if (min == (M.at<int>(x - 1, y) + cu)) {
								//if min = cu > k = 2
								K.at<int>(x, y) = 2;
							}
							else if (min == (M.at<int>(x - 1, y + 1) + cr)) {
								//if min = cr > k = 3
								K.at<int>(x, y) = 3;
							}
						}
						M.at<int>(x, y) = min;
						// all elements in first row of K matrix are 0
						//cout << M.at<int>(x, y) << " ";
					}
				}
				//cout << endl;
			}

			//cout << M << endl;
			//cout << K << endl;

			//Find the best seam in the vertical direction
			// *** WRITE YOUR CODE ***
			tCol = (Mat_<int>(1, height));
			int min = M.at<int>(height - 1, 0);
			int heightCount = height - 1;
			tCol.at<int>(0, heightCount) = 0;
			//cout << tCol;
			for (y = 0; y < width; y++) {
				if (M.at<int>(height - 1, y) < min) {
					min = M.at<int>(height - 1, y);
					tCol.at<int>(0, heightCount) = y;
				}
			}
			//printf("Best seam is %d at index %d\n", min, tCol.at<int>(0, heightCount));
			//cout << tCol << endl;

			//printf("heightCount %d\n", heightCount);

			//cout << K.at<int>(heightCount, tCol.at<int>(0, heightCount)) << endl;

			while (heightCount != 0) {
				if (K.at<int>(heightCount, tCol.at<int>(0, heightCount)) == 1) {
					tCol.at<int>(0, heightCount - 1) = tCol.at<int>(0, heightCount) - 1;
				}
				else if (K.at<int>(heightCount, tCol.at<int>(0, heightCount)) == 2) {
					tCol.at<int>(0, heightCount - 1) = tCol.at<int>(0, heightCount);
				}
				else if (K.at<int>(heightCount, tCol.at<int>(0, heightCount)) == 3) {
					tCol.at<int>(0, heightCount - 1) = tCol.at<int>(0, heightCount) + 1;
				}
				heightCount--;
			}
			//cout << tCol << endl;
		}

		//'w' => Increase height, 's' => reduce height
		if (c == 115 || c == 119) {
			//printf("119 or 115 is pressed\n");
			//Construct M matrix and K matrix in horizontal direction
			// *** WRITE YOUR CODE ***
			/*Mat rotimg;
			rotate(grayimg_border, rotimg, ROTATE_90_CLOCKWISE);
			cout << "normal padding" << endl;
			cout << grayimg_border << endl;
			cout << "rotimg" << endl;
			M = (Mat_<int>(rotimg.rows - 2, rotimg.cols - 2));
			K = (Mat_<int>(rotimg.rows - 2, rotimg.cols - 2));
			cout << "Rotimg Width " << rotimg.cols << " Height " << rotimg.rows << endl;
			cout << "M Width" << M.cols << "Height" << M.rows << endl;
			cout << "K Width" << K.cols << "Height" << K.rows << endl;
			cout << rotimg << endl;*/

			int x, y;
			//cout << grayimg_border << endl;
			/*for (y = 0; y < grayimg_border.cols - 2; y++)
			{
				for (x = 0; x < grayimg_border.rows - 2; x++) {
					printf("Y%d|X%d ", y, x);
				}
				printf("\n");
			}*/
			for (y = 0; y < grayimg_border.cols - 2; y++)
			{
				for (x = 0; x < grayimg_border.rows - 2; x++) {
					//cout << x << "|" << y << " ";
					//if it's the first row > just calculate difference of left/right
					if (y == 0) {
						//printf("%d,%d calculated", x, y);
						M.at<int>(x, y) = abs((int)grayimg_border.at<uchar>(x, y + 1) - (int)grayimg_border.at<uchar>(x + 2, y + 1));
						// all elements in first row of K matrix are 0
						K.at<int>(x, y) = 0;
						//cout << M.at<int>(x, y) << " ";
					}
					//else just calculate cl,cu,cr
					else {
						int cl = abs((int)grayimg_border.at<uchar>(x + 1, y) - (int)grayimg_border.at<uchar>(x + 2, y + 1)) + abs((int)grayimg_border.at<uchar>(x, y + 1) - (int)grayimg_border.at<uchar>(x + 2, y + 1));
						int cu = abs((int)grayimg_border.at<uchar>(x, y + 1) - (int)grayimg_border.at<uchar>(x + 2, y + 1));
						int cr = abs((int)grayimg_border.at<uchar>(x + 1, y) - (int)grayimg_border.at<uchar>(x, y + 1)) + abs((int)grayimg_border.at<uchar>(x, y + 1) - (int)grayimg_border.at<uchar>(x + 2, y + 1));
						//printf("X%d|Y%d|CR|%d-%d + %d-%d\n", x, y, (int)grayimg_border.at<uchar>(x + 1, y), (int)grayimg_border.at<uchar>(x, y + 1), (int)grayimg_border.at<uchar>(x, y + 1), (int)grayimg_border.at<uchar>(x + 2, y + 1));
						//printf("X%d|Y%d|%d|%d|%d|\n",x,y, cl, cu, cr);
						int min;
						//if x == lastrow > no M @ CL > not considering CL to find min
						if (x == height - 1) {
							min = std::min({ M.at<int>(x, y - 1) + cu, M.at<int>(x - 1, y - 1) + cr });
							if (min == (M.at<int>(x, y - 1) + cu)) {
								/*printf("M is %d @ x:%d | y:%d\n", min, x,y);*/
								K.at<int>(x, y) = 2;
							}
							else if (min == (M.at<int>(x - 1, y - 1) + cr)) {
								K.at<int>(x, y) = 3;
							}
						}
						//if y == last col > no M @ CR > not considering CR to find min
						else if (x == 0) {
							//cout << endl << M << endl;
							min = std::min({ M.at<int>(x + 1, y - 1) + cl, M.at<int>(x, y - 1) + cu });
							//printf("||M@%d,%d %d||\n",x+1,y-1, M.at<int>(x + 1, y - 1));
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
						// all elements in first row of K matrix are 0
						//cout << min << " ";
					}
				}
				//cout << endl;
			}

			//cout << M << endl;
			//cout << K << endl;

			//Find the best seam in the horizontal direction
			// *** WRITE YOUR CODE ***
			tRow = (Mat_<int>(1, width));
			int min = M.at<int>(0, width - 1);
			int widthCount = width - 1;
			tRow.at<int>(0, widthCount) = 0;
			//cout << tRow << endl;
			//cout << widthCount << endl;
			//cout << "Width " << width << " Height " << height << endl;
			for (x = 0; x < height; x++) {
				//printf("%d < %d ?", M.at<int>(x, width - 1), min);
				if (M.at<int>(x, width - 1) <= min) {
					min = M.at<int>(x, width - 1);
					tRow.at<int>(0, widthCount) = x;
					//printf(" YES | Updated current index to %d for %d\n", x, min);
					//cout << tRow << endl;
				}
				//printf(" NO\n");
			}
			//cout << tRow << endl;
			//printf("Best seam is %d at index %d\n", min, tRow.at<int>(0, widthCount));
			//cout << tRow << endl;

			//printf("heightCount %d\n", heightCount);

			//cout << K.at<int>(heightCount, tCol.at<int>(0, heightCount)) << endl;

			while (widthCount != 0) {
				//printf("Seeking index : %d\n", tRow.at<int>(0, widthCount));
				//printf("K = %d", K.at<int>(tRow.at<int>(0, widthCount), widthCount));
				if (K.at<int>(tRow.at<int>(0, widthCount), widthCount) == 1) {
					tRow.at<int>(0, widthCount - 1) = tRow.at<int>(0, widthCount) + 1;
				}
				else if (K.at<int>(tRow.at<int>(0, widthCount), widthCount) == 2) {
					tRow.at<int>(0, widthCount - 1) = tRow.at<int>(0, widthCount);
				}
				else if (K.at<int>(tRow.at<int>(0, widthCount), widthCount) == 3) {
					tRow.at<int>(0, widthCount - 1) = tRow.at<int>(0, widthCount) - 1;
				}
				widthCount--;
			}
			//cout << tRow << endl;
			//rotate index of col to row
			/*for (int i = 0; i < height; i++) {
				tRow.at<int>(0, i) = abs((height-1) - tRow.at<int>(0, i));
			}*/
			//cout << tRow << endl;

			/*int x, y;
			for (x = 0; x < grayimg_border.cols - 2; x++) {
				for (y = 0; y < grayimg_border.rows - 2; y++) {
					if (y == 0) {
						M.at<int>(x, y) = abs((int)grayimg_border.at<uchar>(x, y + 1) - (int)grayimg_border.at<uchar>(x + 2, y + 1));
						K.at<int>(x, y) = 0;
					}
					else {
						int cl = abs((int)grayimg_border.at<uchar>(x, y) - (int)grayimg_border.at<uchar>(x + 1, y + 1)) + abs((int)grayimg_border.at<uchar>(x + 1, y + 1) - (int)grayimg_border.at<uchar>(x, y + 1));
						int cu = abs((int)grayimg_border.at<uchar>(x, y + 1) - (int)grayimg_border.at<uchar>(x + 1, y));
						int cr = abs((int)grayimg_border.at<uchar>(x, y + 2) - (int)grayimg_border.at<uchar>(x + 1, y)) + abs((int)grayimg_border.at<uchar>(x + 1, y + 2) - (int)grayimg_border.at<uchar>(x + 1, y));
					}
				}
			}*/
			//Find the best seam in the horizontal direction
			// *** WRITE YOUR CODE ***

		}

		//Insert or delete the best seam
		if (c == 97) {
			//Reduce width or delete seam verticaly
			//Copy the pixels into this image
			Mat img_new(height, --width, CV_8UC3, Scalar(0, 0, 0));
			// *** WRITE YOUR CODE ***
			//cout << tCol << endl;
			int i, j;
			bool shift = false;
			//printf("\n\n new image %d*%d \n", width, height);
			for (i = 0; i < height; i++) {
				int seamIndex = tCol.at<int>(0, i);
				shift = false;
				for (j = 0; j <= width; j++) {
					if (j == seamIndex) {
						shift = true;
						continue;
					}

					if (shift == true) {
						//cout << "SSS" << endl;
						//printf("%d,%d ", i, j);
						img_new.at<Vec3b>(i, j - 1) = img.at<Vec3b>(i, j);
					}
					else
					{
						//cout << img.at<Vec3b>(Point(i, j)) << endl;
						//printf("%d|%d ", i, j);
						img_new.at<Vec3b>(i, j) = img.at<Vec3b>(i, j);
					}
				}
				//printf("\n");
			}

			//Show the resized image
			imshow("Example", img_new);
			//Clone img_new into ing for the nest loop processing
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
			bool shift = false;
			//printf("\n\n new image %d*%d \n", width, height);
			for (i = 0; i < height; i++) {
				int seamIndex = tCol.at<int>(0, i);
				shift = false;
				for (j = 0; j < width; j++) {
					if (j == seamIndex) {
						shift = true;
						//printf("%d,%d ", i, j);
						img_new.at<Vec3b>(i, j) = img.at<Vec3b>(i, j);
						continue;

					}

					if (shift == true) {
						//cout << "SSS" << endl;
						//printf("%d,%d ", i, j);
						img_new.at<Vec3b>(i, j) = img.at<Vec3b>(i, j - 1);
					}
					else
					{
						//cout << img.at<Vec3b>(Point(i, j)) << endl;
						//printf("%d|%d ", i, j);
						img_new.at<Vec3b>(i, j) = img.at<Vec3b>(i, j);
					}
				}
				printf("\n");
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
			//printf("S is pressed\n");
			Mat img_new(--height, width, CV_8UC3, Scalar(0, 0, 0));
			// *** WRITE YOUR CODE ***
			//cout << tRow << endl;
			int i, j;
			bool shift = false;
			//printf("\n\n new image %d*%d \n", width, height);
			//cout << grayimg << endl;

			for (i = 0; i < width; i++) {
				int seamIndex = tRow.at<int>(0, i);
				shift = false;
				for (j = 0; j < height; j++) {
					if (j == seamIndex) {
						shift = true;
					}
					if (shift == true) {
						//printf("%d|%d >> shift getting >> %d,%d\n", i, j, j + 1, i);
						img_new.at<Vec3b>(j, i) = img.at<Vec3b>(j + 1, i);
					}
					else {
						//printf("%d|%d >> getting >> %d,%d\n", i, j, j, i);
						img_new.at<Vec3b>(j, i) = img.at<Vec3b>(j, i);
					}
				}
				//printf("\n");
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
			//printf("W is pressed");
			//cout << tRow << endl;
			Mat img_new(++height, width, CV_8UC3, Scalar(0, 0, 0));
			// *** WRITE YOUR CODE ***
			cout << tRow << endl;
			int i, j;
			bool shift = false;
			//printf("\n\n new image %d*%d \n", width, height);
			//cout << grayimg << endl;

			for (i = 0; i < width; i++) {
				int seamIndex = tRow.at<int>(0, i);
				shift = false;
				for (j = 0; j < height; j++) {
					if (j == seamIndex) {
						shift = true;
						img_new.at<Vec3b>(j, i) = img.at<Vec3b>(j, i);
						continue;
					}
					if (shift == true) {
						/*printf("%d|%d >> shift getting >> %d,%d\n", j, i, j - 1, i);
						cout << img.at<Vec3b>(j - 1, i) << endl;*/
						img_new.at<Vec3b>(j, i) = img.at<Vec3b>(j - 1, i);
					}
					else {
						/*printf("%d|%d >> getting >> %d,%d\n", j, i, j, i);
						cout << img.at<Vec3b>(j, i) << endl;*/
						img_new.at<Vec3b>(j, i) = img.at<Vec3b>(j, i);
					}
				}
				//printf("\n");
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