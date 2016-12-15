
#include "main.h"
#include "Functions.h"


int main()
{

	//Image from data
	/*
	Mat imagen = imread("Picture 1.jpg");
	if (!imagen.data) {
	cout << "Error abriendo la imagen" << endl;
	return 1;
	}
	*/
	//Image from cam
	/*
	Mat imagen;
	VideoCapture vid(0);
	if (!vid.isOpened()) {
	cout << "Error abriendo el video" << endl;
	return 1;
	}
	vid.read(imagen);
	if (!imagen.data) {
	cout << "Error abriendo la imagen del video" << endl;
	return 1;
	}
	//Gray and binary transformations

	Mat imagen_gray, imagen_bin;
	cvtColor(imagen, imagen_gray, CV_BGR2GRAY);
	threshold(imagen_gray, imagen_bin, 110, 255, THRESH_BINARY);
	//namedWindow("original", CV_WINDOW_AUTOSIZE);
	//namedWindow("gray", CV_WINDOW_AUTOSIZE);
	//namedWindow("bin", CV_WINDOW_AUTOSIZE);
	//imshow("original", imagen);
	//imshow("gray", imagen_gray);
	//imshow("bin", imagen_bin);
	*/

	//Load all templates
	//////TEMPLATE
	//Mat img_K = imread("plantillas//K.jpg");
	//Mat img_A = imread("plantillas//A.jpg");
	//Mat img_corazon = imread("plantillas//corazon.jpg");
	//Mat img_trebol = imread("plantillas//trebol.jpg");
	Mat img_Q = imread("plantillas//Q.jpg");
	Mat img_corazon = imread("plantillas//corazon3.jpg");

	VideoCapture vid(0);
	if (!vid.isOpened()) {
		cout << "Error abriendo el video" << endl;
		return 1;
	}
	vid.set(CV_CAP_PROP_FPS, 100);
	double fps = vid.get(CV_CAP_PROP_FPS);
	cout << "Frames per seconds: " << fps << endl;
	vector <Mat> imagen_filter;
	//namedWindow("imagen", CV_WINDOW_AUTOSIZE);
	while (1) {
		Mat imagen;
		Mat imagen_gray, imagen_bin;
		vector <Mat> vector_cartas;
		bool frame_read = vid.read(imagen);
		if (!frame_read) {
			cout << "Error mostrando el frame" << endl;
			return 1;
		}
		cvtColor(imagen, imagen_gray, CV_BGR2GRAY);
		threshold(imagen_gray, imagen_bin, 110, 255, THRESH_BINARY);


		//If you want to know the corners of your template, activate
		/*
		Mat new_img, new_img_gray, new_img_bin;
		new_img = imread("plantilla_transofrmacion2.jpg", 1);
		cvtColor(new_img, new_img_gray, CV_BGR2GRAY);
		threshold(new_img_gray, new_img_bin, 110, 255, THRESH_BINARY);
		imagen_filter = contour_cards(new_img_bin, new_img);
		*/


		imagen_filter = contour_cards(imagen_bin, imagen, vector_cartas);
		vector <Mat> img_transform;
		vector<vector<Point2f>>esquinas;
		for (int i = 0; i < imagen_filter.size(); i++) {
			esquinas.push_back(houghLines(imagen_filter[i]));
		}
		int radius = 5;
		for (int i = 0; i < imagen_filter.size(); i++) {
			circle(imagen, esquinas[i][0], radius, Scalar(0, 255, 0), 0, 8);
			circle(imagen, esquinas[i][1], radius, Scalar(0, 255, 0), 0, 8);
			circle(imagen, esquinas[i][2], radius, Scalar(0, 255, 0), 0, 8);
			circle(imagen, esquinas[i][3], radius, Scalar(0, 255, 0), 0, 8);
		}

		imshow("image_window", imagen);

		for (int i = 0; i < imagen_filter.size(); i++) {
			img_transform.push_back(transformation(vector_cartas[i], esquinas[i]));
		}

		for (int i = 0; i < img_transform.size(); i++) {
			string v = "transform" + i;
			imshow(v, img_transform[i]);
		}
		vector<Mat> img_crop, img_crop_symbol;
		img_crop = crop_vector_image(img_transform);
		img_crop_symbol = crop_symbol(img_crop);

		waitKey(0);
		

		//Aplicamos blob


		//Template 1
		/*
		vector<Mat> template_palos;
		template_palos.push_back(img_corazon);
		//template_palos.push_back(img_trebol);

		vector<Mat> template_numeros;
		//template_numeros.push_back(img_K);
		//template_numeros.push_back(img_A);
		template_numeros.push_back(img_Q);

		Point  Point_TemplateLocation;
		for (int i = 0; i < template_palos.size(); i++)
		{

			if (!FindTemplate(img_crop[0], template_palos[i], Point_TemplateLocation))
			{
				cout << "No Match Found";
			}
			/// Show me what you got
			rectangle(img_crop[0], Point_TemplateLocation, Point(Point_TemplateLocation.x + template_palos[i].cols, Point_TemplateLocation.y + template_palos[i].rows), Scalar(0, 0, 255), 2, 8, 0);
			putText(img_crop[0], format("Palo"), Point(Point_TemplateLocation.x + template_palos[i].cols / 4, Point_TemplateLocation.y + template_palos[i].rows / 2), 1, 1, Scalar(255, 0, 0), 1, -1);

		}

		Point  Point_TemplateLocation2;
		for (int i = 0; i < template_palos.size(); i++)
		{
			if (!FindTemplate(img_crop[0], template_numeros[i], Point_TemplateLocation2))
			{
				cout << "No Match Found";
			}
			/// Show me what you got
			rectangle(img_crop[0], Point_TemplateLocation2, Point(Point_TemplateLocation2.x + template_numeros[i].cols, Point_TemplateLocation2.y + template_numeros[i].rows), Scalar(0, 255, 0), 2, 8, 0);
			putText(img_crop[0], format("Numero", i), Point(Point_TemplateLocation.x + template_numeros[i].cols / 4, Point_TemplateLocation.y + template_numeros[i].rows / 2), 1, 1, Scalar(255, 0, 0), 1, -1);
		}

		namedWindow("image_window", CV_WINDOW_AUTOSIZE);
		//namedWindow("result_window", CV_WINDOW_AUTOSIZE);
		imshow("image_window", img_crop[0]);
		//imshow("image_window", Img_Template_2);
		//imshow("result_window", Img_Result);

		/*
		cv::Mat input = img_transform[0];

		cv::Mat gray;
		cv::cvtColor(input, gray, CV_BGR2GRAY);

		cv::Mat templ = img_rey;

		cv::Mat img = input;
		cv::Mat result;
		/// Create the result matrix
		int result_cols = img.cols - templ.cols + 1;
		int result_rows = img.rows - templ.rows + 1;

		result.create(result_cols, result_rows, CV_32FC1);

		int match_method = CV_TM_SQDIFF;

		/// Do the Matching and Normalize
		matchTemplate(img, templ, result, match_method);
		normalize(result, result, 0, 1, cv::NORM_MINMAX, -1, cv::Mat());

		/// Localizing the best match with minMaxLoc
		double minVal; double maxVal; cv::Point minLoc; cv::Point maxLoc;
		cv::Point matchLoc;

		minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc, cv::Mat());

		/// For SQDIFF and SQDIFF_NORMED, the best matches are lower values. For all the other methods, the higher the better
		if (match_method == CV_TM_SQDIFF || match_method == CV_TM_SQDIFF_NORMED)
		{
		matchLoc = minLoc;
		}
		else
		{
		matchLoc = maxLoc;
		}

		/// Show me what you got
		cv::rectangle(input, matchLoc, cv::Point(matchLoc.x + templ.cols, matchLoc.y + templ.rows), cv::Scalar::all(0), 2, 8, 0);
		cv::rectangle(result, matchLoc, cv::Point(matchLoc.x + templ.cols, matchLoc.y + templ.rows), cv::Scalar::all(0), 2, 8, 0);


		cv::imshow("input", input);
		cv::imshow("template", templ);

		//cv::imwrite("../outputData/TemplateMatch.jpg", input);
		*/



		if (waitKey(30) == 27) {

			cout << "Esc key pressed" << endl;
			return 1;
		}
	}
	//destroyWindow("imagen");

	return 0;



	//Sobel
	//Mat imagen_bordes, new_bin;
	//sobel(imagen_gray, imagen_bordes);
	//binaring(imagen_bordes, new_bin);

	//External countour
	//Mat contorno;
	//external_contours(imagen_gray, contorno);

	// Floodfill from point (0, 0)
	//Mat im_floodfill = imagen_bin.clone();
	//floodFill(im_floodfill, cv::Point(0, 0), Scalar(255));
	//Mat im_floodfill_inv;
	//bitwise_not(im_floodfill, im_floodfill_inv);
	//Mat im_out = (imagen_bin | im_floodfill_inv);
	//imshow("Floodfilled Image", im_floodfill);
	//imshow("Inverted Floodfilled Image", im_floodfill_inv);
	//imshow("Foreground", im_out);

	//Hough lines
	//Mat bordes;
	//houghLines(im_out, bordes);




	//contour_cards(imagen_bin, imagen);


	waitKey(0);
	destroyAllWindows();
	return 0;
}

