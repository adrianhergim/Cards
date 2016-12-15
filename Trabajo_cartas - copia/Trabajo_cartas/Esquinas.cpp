#pragma once
#include "main.h"
#include "Esquinas.h"
#include "Matem.h"


template <typename T> vector<size_t> sort_indexes(const vector<T> &v);

vector <Point2f>houghLines(const Mat & img)
{
	Mat bordes;
	//Aplicamos canny

	Canny(img, bordes, 225, 255, 3);

	namedWindow("bordes", CV_WINDOW_AUTOSIZE);
	imshow("bordes", bordes);

	//Usamos Houg
	Mat bordes_new;
	vector<Vec2f> lines;
	cvtColor(bordes, bordes_new, COLOR_GRAY2BGR);
	HoughLines(bordes, lines, 1, 4 * CV_PI / 180, 30, 0, 0);


	//Encontramos la interseccion de las lineas
	Point2f punto;
	Point2f esquinas[4];
	bool copia;
	int cont = 0, grado;
	vector<float> maxx, minx, maxy, miny;

	for (int i = 0; i < lines.size(); i++)
	{
		//sacamos los puntos par la linea i
		float rho1 = lines[i][0], theta1 = lines[i][1];
		Point2f line1cord1, line1cord2, line2cord1, line2cord2;
		double a = cos(theta1), b = sin(theta1);
		double x0 = a*rho1, y0 = b*rho1;
		line1cord1.x = cvRound(x0 + 1000 * (-b));
		line1cord1.y = cvRound(y0 + 1000 * (a));
		line1cord2.x = cvRound(x0 - 1000 * (-b));
		line1cord2.y = cvRound(y0 - 1000 * (a));
		line(bordes_new, line1cord1, line1cord2, Scalar(0, 0, 255), 1, LINE_AA); // Para dibujar las lineas
		for (int j = i + 1; j < lines.size(); j++)
		{

			//Sacamos los puntos para la linea j
			float rho2 = lines[j][0], theta2 = lines[j][1];
			double a2 = cos(theta2), b2 = sin(theta2);
			double x02 = a2*rho2, y02 = b2*rho2;
			line2cord1.x = cvRound(x02 + 1000 * (-b2));
			line2cord1.y = cvRound(y02 + 1000 * (a2));
			line2cord2.x = cvRound(x02 - 1000 * (-b2));
			line2cord2.y = cvRound(y02 - 1000 * (a2));
			lineIntersection(line1cord1, line1cord2, line2cord1, line2cord2, punto);
			cout << punto << endl;
			if (punto.x >= 0 && punto.y >= 0 && punto.x < img.cols && punto.y < img.rows) //Comprobar que el punto este en la iamgen
			{
				int radius = 5;

				//Hacemos qe no haya mas de una esquina repetida
				copia = true;
				for (int w = 0; w < 4; w++)
				{
					if (punto.x == esquinas[w].x && punto.y == esquinas[w].y)
						copia = false;
					if (punto.x == 0 && punto.y == 0)
						copia = false;
					if (cont > 4)
						copia = false;

					grado = abs(grados(line1cord1, punto, line2cord1));
					if (grado < 50 || grado > 100)
					{
						copia = false;
						circle(bordes_new, punto, radius, Scalar(255, 255, 0), -1, 8);
						//cout << " grados:" << grado << endl;
					}
				}
				for (int k = 0; k < maxx.size(); k++)
				{
					if (punto.x < maxx[k] && punto.x > minx[k] && punto.y < maxy[k] && punto.y > miny[k])
					{
						copia = false;
						esquinas[k].x = (esquinas[k].x + punto.x) / 2;
						esquinas[k].y = (esquinas[k].y + punto.y) / 2;
						cout << "andale cabron" << esquinas[k] << "  " << k << endl;
						circle(bordes_new, punto, radius, Scalar(255, 0, 0), -1, 8);
					}
				}
				if (copia == true)
				{

					esquinas[cont] = punto;

					maxx.push_back(punto.x + 25);
					minx.push_back(punto.x - 25);
					maxy.push_back(punto.y + 25);
					miny.push_back(punto.y - 25);
					cout << "esquina" << cont << " : " << esquinas[cont] << " grado esquina: " << grado << endl;
					cont++;

				}
			}

		}

	}
	//Nos quedamos con la esquina superior y la inferior
	Point2f esquinasup, esquinainf;
	int max = 0, min = 0, radius2 = 5;
	float acumpos = esquinas[0].y, acummin = esquinas[0].y;
	for (int i = 0; i < 4; i++)
	{
		if (esquinas[i].y > acumpos)
		{
			acumpos = esquinas[i].y;
			max = i;
		}

		if (esquinas[i].y < acummin)
		{
			acummin = esquinas[i].y;
			min = i;
		}
		cout << "esquina" << i << " : " << esquinas[i] << endl;
		circle(bordes_new, esquinas[i], radius2, Scalar(0, 255, 0), -1, 8);
	}

	//Ordenamos las esquinas

	vector<Point2f> esquinasor;
	vector<float> distancias;
	esquinasor.resize(4);


	for (int i = 0; i < 4; i++)
	{

		distancias.push_back(sqrt(pow(abs((esquinas[min].x - esquinas[i].x)), 2) + pow(abs((esquinas[min].y - esquinas[i].y)), 2)));
	}
	for (int i = 0; i < distancias.size(); ++i)
	{
		std::cout << "distancias " << distancias[i] << ' ' << endl;
	}

	vector<size_t> indices = sort_indexes(distancias);


	if (esquinas[min].x < esquinas[indices[1]].x)
	{
		esquinasor[0] = esquinas[min];
		esquinasor[1] = esquinas[indices[1]];
		esquinasor[2] = esquinas[indices[2]];
		esquinasor[3] = esquinas[indices[3]];
	}
	else if (esquinas[min].x > esquinas[indices[1]].x)
	{
		esquinasor[0] = esquinas[indices[1]];
		esquinasor[1] = esquinas[min];
		esquinasor[2] = esquinas[indices[3]];
		esquinasor[3] = esquinas[indices[2]];
	}

	for (int i = 0; i < esquinasor.size(); ++i)
	{
		std::cout << "esquinasor " << esquinasor[i] << ' ' << endl;
	}

	int radius = 35;
	circle(bordes_new, esquinasor[0], radius, Scalar(255, 0, 0), 0, 8);
	circle(bordes_new, esquinasor[1], radius, Scalar(0, 255, 0), 0, 8);
	circle(bordes_new, esquinasor[2], radius, Scalar(0, 0, 255), 0, 8);
	circle(bordes_new, esquinasor[3], radius, Scalar(255, 255, 0), 0, 8);

	imshow("detected_lines", bordes_new);


	return esquinasor;
}





template <typename T> vector<size_t> sort_indexes(const vector<T> &v)
{

	// initialize original index locations
	vector<size_t> idx(v.size());
	iota(idx.begin(), idx.end(), 0);

	// sort indexes based on comparing values in v
	sort(idx.begin(), idx.end(),
		[&v](size_t i1, size_t i2) {return v[i1] < v[i2]; });

	return idx;
}
