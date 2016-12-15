#pragma once
#include "main.h"
#include "Image.h"





void binaring(const Mat & imagen, Mat & new_bin) {

	threshold(imagen, new_bin, 50, 255, THRESH_BINARY);
	namedWindow("new_bin", CV_WINDOW_AUTOSIZE);
	imshow("new_bin", new_bin);
}









Mat transformation(Mat & img, vector <Point2f> puntos) {


	Mat im_src = img;
	// Four corners of the book in source image
	vector<Point2f> pts_src;

	for (int i = 0; i < puntos.size(); i++) {
		pts_src.push_back(puntos[i]);
		circle(im_src, pts_src[i], 1, Scalar(0, 255, 0), 5, 8, 0);
	}


	// Read destination image.
	Mat im_dst = imread("plantilla_transofrmacion2.jpg");
	// Four corners of the book in destination image.
	vector<Point2f> pts_dst;

	pts_dst.push_back(Point2f(3, 4.91246));
	pts_dst.push_back(Point2f(243, 3.49175));
	pts_dst.push_back(Point2f(4, 335.043));
	pts_dst.push_back(Point2f(241.5, 338.586));


	circle(im_dst, Point(3, 4.91246), 1, Scalar(0, 255, 0), 5, 8, 0);
	circle(im_dst, Point(243, 3.49175), 1, Scalar(0, 255, 0), 5, 8, 0);
	circle(im_dst, Point(4, 335.043), 1, Scalar(0, 255, 0), 5, 8, 0);
	circle(im_dst, Point(241.5, 338.586), 1, Scalar(0, 255, 0), 5, 8, 0);

	// Calculate Homography
	Mat h = findHomography(pts_src, pts_dst);

	// Output image
	Mat im_out;
	// Warp source image to destination based on homography
	warpPerspective(im_src, im_out, h, im_dst.size());


	// Display images
	//imshow("Source Image", im_src);
	//imshow("Destination Image", im_dst);
	//imshow("Warped Source Image", im_out);
	return im_out;

}






