#include "Contour.h"

vector <Mat> contour_cards(const Mat &imagen_bin, const Mat &imagen, vector <Mat> &resultado_resta) {
	//Use this function to clean all except cards

	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;

	findContours(imagen_bin, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
	double area;
	double largest_area = 0;

	vector <int> largest_contour_index;

	for (size_t i = 0; i < contours.size(); i++) // iterate through each contour.
	{
		area = contourArea(contours[i]);
		if (area > largest_area)
		{
			largest_area = area;
		}
	}
	double prox_contours = 80 * largest_area / 100;  //Range for more cards - X% from max
	for (size_t i = 0; i < contours.size(); i++) // iterate through each contour.
	{
		area = contourArea(contours[i]);		//Watch out with the -X%, of the largest area, to identify items with the same level
		if (area > prox_contours) //Not only marking the biggest, mark the biggest and all of them which have -X% from max
		{
			largest_contour_index.push_back(i);               //Store the index of these contours
															  //bounding_rect = boundingRect(contours[i]);

		}
	}
	vector <Mat> imagen_cards, imagen_cards_contour, imagen_cards_white;
	cout << "area = " << area << endl;


	for (int i = 0; i < largest_contour_index.size(); i++) {

		Mat new_image_contour, new_image;
		imagen.copyTo(new_image_contour);
		imagen.copyTo(new_image);

		//Contorno de cada una de las cartas
		drawContours(new_image_contour, contours, largest_contour_index[i], Scalar(0, 255, 0), 2); // Draw the largest contour using previously stored index.
		imagen_cards_contour.push_back(new_image_contour);

		//Relleno de cada una de las cartas, para individualizarlas
		drawContours(new_image, contours, largest_contour_index[i], Scalar(0, 0, 0), -1); // Draw the largest contour using previously stored index.
		imagen_cards.push_back(new_image);

	}


	for (int i = 0; i < imagen_cards.size(); i++) {
		Mat result = imagen - imagen_cards[i];
		resultado_resta.push_back(result);
		string v = "carta " + i;
		imshow(v, resultado_resta[i]);
	}
	for (int i = 0; i < largest_contour_index.size(); i++) {
		//Relleno de cada una de las cartas obtener el relleno blanco
		Mat	new_image_white;

		resultado_resta[i].copyTo(new_image_white);
		drawContours(new_image_white, contours, largest_contour_index[i], Scalar(255, 255, 255), -1); // Draw the largest contour using previously stored index.
		imagen_cards_white.push_back(new_image_white);
		string v = "rect " + i;
		imshow(v, imagen_cards_white[i]);
	}

	return imagen_cards_white;
}


void external_contours(const Mat & imagen_gray, Mat & contourImage)
{
	//Prepare the image for findContours
	Mat imagen_bin;
	cv::threshold(imagen_gray, imagen_bin, 75, 255, CV_THRESH_BINARY);

	//Find the contours. Use the contourOutput Mat so the original image doesn't get overwritten
	std::vector<std::vector<cv::Point> > contours;
	cv::Mat contourOutput = imagen_bin.clone();
	cv::Mat contourOutput2 = imagen_bin.clone();
	cv::findContours(contourOutput, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);

	//Draw the contours
	contourImage = Mat(imagen_bin.size(), CV_8UC3, cv::Scalar(0, 0, 0));
	cv::Scalar colors[3];
	colors[0] = cv::Scalar(255, 0, 0);
	colors[1] = cv::Scalar(0, 255, 0);
	colors[2] = cv::Scalar(0, 0, 255);
	for (size_t idx = 0; idx < contours.size(); idx++) {
		cv::drawContours(contourImage, contours, idx, colors[idx % 3]);
	}
	cv::imshow("Contours", contourImage);
	cvMoveWindow("Contours", 200, 0);
}