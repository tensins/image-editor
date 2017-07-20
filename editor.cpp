#include <opencv2/opencv.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include <vector>
#include <cmath>
#include <string>

using namespace std;
using namespace cv;

// a program for image editing
// whether for fun or as a tool for computer vision to find right parameters for various
// situations

Mat img;
int blur_val = 0;
const int blur_max = 100;
int brightness_val = 50;
const int brightness_max = 100;
int hsv_hval = 0;
int contrast_val = 100;
const int contrast_max = 200;


const string trackbar_window = "Controls";
const string img_window = "Image";
int kern_size = 0;

void applyFilters(int pos, void*) {
	Mat bright_img;
	Mat blurred_img;
	Mat contrast_img;

	// apply brightness
	img.copyTo(bright_img);
	Vec3i bright_adder;
	bright_adder[0] = (brightness_val-50);
	bright_adder[1] = (brightness_val-50);
	bright_adder[2] = (brightness_val-50);
	bright_img += bright_adder;

	// apply contrast
	// idea is to degrease intensity of pixels that already have low intensity and
	// increase the intensity of pixels that have higher intensities
	bright_img.copyTo(contrast_img);
	for (int i = 0; i < contrast_img.rows; i++) {
		for (int j = 0; j < contrast_img.cols; j++) {
			Vec3b pix = contrast_img.at<Vec3b>(Point(j,i));
			double b = pix[0]/255.0;
			double g = pix[1]/255.0;
			double r = pix[2]/255.0;
			b = (b <= 0.5)?
				0.5 * pow(2.0 * b, contrast_val/100.0):
				1 - 0.5 * pow(2.0 * (1-b), contrast_val/100.0);
			g = (g <= 0.5)?
				0.5 * pow(2.0 * g, contrast_val/100.0):
				1 - 0.5 * pow(2.0 * (1-g), contrast_val/100.0);
			r = (r <= 0.5)?
				0.5 * pow(2.0 * r, contrast_val/100.0):
				1 - 0.5 * pow(2.0 * (1-r), contrast_val/100.0);
			pix[0] = b * 255;
			pix[1] = g * 255;
			pix[2] = r * 255;
			contrast_img.at<Vec3b>(Point(j,i)) = pix;
		}
	}

	// apply blur
	if (blur_val) {
		// mean blur
		blur(contrast_img, blurred_img, Size(blur_val*2 + 1, blur_val*2 + 1));
		imshow(img_window, blurred_img);
	} else {
		imshow(img_window, contrast_img);
	}
}


int main(int argc, char ** argv) {

	namedWindow(trackbar_window, WINDOW_AUTOSIZE);
	namedWindow(img_window, WINDOW_AUTOSIZE);

	img = imread(argv[1]);

	if (img.empty()) { return -1; }


	createTrackbar("Blur", trackbar_window, &blur_val, blur_max, applyFilters);
	createTrackbar("Brightness", trackbar_window, &brightness_val, brightness_max, applyFilters);
	createTrackbar("Contrast", trackbar_window, &contrast_val, contrast_max, applyFilters);

	applyFilters(0, NULL);

	waitKey(0);
	return 0;
}