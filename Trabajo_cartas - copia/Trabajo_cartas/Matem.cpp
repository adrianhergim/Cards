#pragma once
#include "Matem.h"



int grados(Point2f a, Point2f b, Point2f c)
{
	Point2f ab = { b.x - a.x, b.y - a.y };
	Point2f cb = { b.x - c.x, b.y - c.y };

	float dot = (ab.x * cb.x + ab.y * cb.y); // dot product
	float cross = (ab.x * cb.y - ab.y * cb.x); // cross product

	float alpha = atan2(cross, dot);

	return (int)floor(alpha * 180. / CV_PI + 0.5);
}

bool lineIntersection(const cv::Point2f &a1, const cv::Point2f &b1, const cv::Point2f &a2, const cv::Point2f &b2, cv::Point2f &intersection) {
	double A1 = b1.y - a1.y;
	double B1 = a1.x - b1.x;
	double C1 = (a1.x * A1) + (a1.y * B1);

	double A2 = b2.y - a2.y;
	double B2 = a2.x - b2.x;
	double C2 = (a2.x * A2) + (a2.y * B2);

	double det = (A1 * B2) - (A2 * B1);


	intersection.x = static_cast<float>(((C1 * B2) - (C2 * B1)) / (det));
	intersection.y = static_cast<float>(((C2 * A1) - (C1 * A2)) / (det));
	return true;



}

