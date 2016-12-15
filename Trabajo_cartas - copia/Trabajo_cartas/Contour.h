#pragma once
#include "main.h"
void external_contours(const Mat & imagen_gray, Mat & imagen_bordes);
vector <Mat> contour_cards(const Mat &imagen_bin, const Mat &imagen, vector <Mat> & resultado_resta);