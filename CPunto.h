#pragma once

#include <iostream>
#include<math.h>
#include <iomanip>
using namespace std;

const int precision =17;

class CPunto
{
public:
	long double x;
	long double y;
	int cluster_pertenece;

	CPunto(long double x, long double y) {
		this->x = x;
		this->y = y;
		cluster_pertenece = 0;
	}

	long double distance_euclidiana(CPunto k_i) {
		return pow((k_i.x - x),2) + pow((k_i.y - y),2);
	}

	void print() {
		cout << setprecision(precision) << "Eje X(latitud) " << x << endl;
		cout << setprecision(precision) <<"Eje Y(altitud) " << y << endl;
		cout <<  "Cluster Pertenencia : " << cluster_pertenece << endl;
		cout << endl;
	}

	void print_c() {
		cout << setprecision(precision) << "Eje X(latitud) " << x << endl;
		cout << setprecision(precision) << "Eje Y(altitud) " << y << endl;
		cout << endl;
	}

	//Solo para centroides
	void update_coordenadas(long double x, long double y) {
		this->x = x;
		this->y = y;
	}

};

