#include "Crop.h"

vector <Mat> crop_vector_image(const vector<Mat> & vector_img) {

	Rect myROI(0, 0, 40, 115);
	vector <Mat> img_crop;

	for (int i = 0; i < vector_img.size(); i++) {
		img_crop.push_back(vector_img[i](myROI));
	}
	for (int i = 0; i < vector_img.size(); i++) {
		string v = "cortada" + i;
		imshow(v, img_crop[i]);
	}
	return img_crop;
}

vector <Mat> crop_symbol(const vector<Mat> & vector_img) {
	///Capturamos imagen desde archivo


	int valorarea = 200;
	int valorcircular = 12;
	int valorconvexo = 79;
	int valorinercia = 17;



	SimpleBlobDetector::Params params;


	// Change thresholds
	params.minThreshold = 10;
	params.maxThreshold = 200;

	// Filter by Area.
	params.filterByArea = true;
	params.minArea = valorarea;

	// Filter by Circularity
	params.filterByCircularity = true;
	params.minCircularity = (float)valorcircular / 100;

	// Filter by Convexity
	params.filterByConvexity = true;
	params.minConvexity = (float)valorconvexo / 100;

	// Filter by Inertia
	params.filterByInertia = true;
	params.minInertiaRatio = (float)valorinercia / 100;

	vector<KeyPoint> keypoints;
	vector <Mat> img_crop;

	Ptr<SimpleBlobDetector> detector = SimpleBlobDetector::create(params);

	for (int i = 0; i < vector_img.size(); i++)
	{
		detector->detect(vector_img[i], keypoints);
		Mat im_with_keypoints;
		int topleftx, toplefty;
		topleftx = keypoints[0].pt.x - 12;
		toplefty = keypoints[0].pt.y - 12;
		Rect myROI(topleftx, toplefty, 25, 30);
		img_crop.push_back(vector_img[i](myROI));

	}
	for (int i = 0; i < vector_img.size(); i++) {
		string v = "simbolo" + i;
		imshow(v, img_crop[i]);
	}

	return img_crop;

}

