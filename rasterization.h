#ifndef RASTERIZATION_H
#define RASTERIZATION_H

#include <algorithm>
#include <cmath>
#include "vec.h"
#include "geometry.h"
#include <iostream>
#include <list>

//////////////////////////////////////////////////////////////////////////////

struct Pixel{
	int x, y;
};

inline Pixel toPixel(vec2 u){
	return { (int)round(u[0]), (int)round(u[1]) };
}

inline vec2 toVec2(Pixel p){
	return {(float)p.x, (float)p.y};
}

//////////////////////////////////////////////////////////////////////////////

template<class Line>
std::vector<Pixel> rasterizeLine(const Line& P){
	//return simple(P[0], P[1]);

	return dda(P[0], P[1]);

	//return bresenham(toPixel(P[0]), toPixel(P[1]));
}

//////////////////////////////////////////////////////////////////////////////

inline std::vector<Pixel> simple(vec2 A, vec2 B){
	std::vector<Pixel> out;
	vec2 d = B - A;
	float m = d[1]/d[0];
	float b = A[1] - m*A[0];

	int x0 = round(A[0]);
	int x1 = round(B[0]);

	for(int x=x0; x <= x1; x++){
		int y = round(m*x + b);
		out.push_back({x, y});
	}
	return out;
}

//////////////////////////////////////////////////////////////////////////////

inline std::vector<Pixel> dda(vec2 A, vec2 B){
	vec2 dif = B - A;
	float delta = std::max(fabs(dif[0]), fabs(dif[1]));

	vec2 d = (1/delta)*dif;
	vec2 p = A;

	std::vector<Pixel> out;
	for(int i = 0; i <= delta; i++){
		out.push_back(toPixel(p));
		p = p + d;
	}
	return out;
}

//////////////////////////////////////////////////////////////////////////////

inline std::vector<Pixel> bresenham_base(int dx, int dy){
	std::vector<Pixel> out;

	int D = 2*dy - dx; 
	int y = 0;
	for(int x = 0; x <= dx; x++){
		out.push_back({x, y});
		if(D > 0){
			y++;
			D -= 2*dx;
		}
		D += 2*dy;
	}
	return out;
}

inline std::vector<Pixel> bresenham(int dx, int dy){
	std::vector<Pixel> out;

	if(dx >= dy){
		out = bresenham_base(dx, dy);
	}else{
		out = bresenham_base(dy, dx);
		for(Pixel& p: out)
			p = {p.y, p.x};
	}
	return out;
}

inline std::vector<Pixel> bresenham(Pixel p0, Pixel p1){
	if(p0.x > p1.x)
		std::swap(p0, p1);

	std::vector<Pixel> out = bresenham(p1.x - p0.x, abs(p1.y - p0.y));

	int s = (p0.y <= p1.y)? 1: -1;

	for(Pixel& p: out)
		p = {p0.x + p.x, p0.y + s*p.y};

	return out;
}

//////////////////////////////////////////////////////////////////////////////

template<class Tri>
std::vector<Pixel> rasterizeTriangle(const Tri& P){
	return simple_rasterize_triangle(P);
	//return scanline(P);
}

template<class Tri>
std::vector<Pixel> simple_rasterize_triangle(const Tri& P){
	vec2 A = P[0];
	vec2 B = P[1];
	vec2 C = P[2];

	int xmin =  ceil(std::min({A[0], B[0], C[0]}));
	int xmax = floor(std::max({A[0], B[0], C[0]}));
	int ymin =  ceil(std::min({A[1], B[1], C[1]}));
	int ymax = floor(std::max({A[1], B[1], C[1]}));

	std::vector<Pixel> out;
	Pixel p;
	for(p.y = ymin; p.y <= ymax; p.y++)
		for(p.x = xmin; p.x <= xmax; p.x++)
			if(is_inside(toVec2(p), P))
				out.push_back(p);
	return out;
}

struct Reta{
	float a, b;
	bool vertical = false;
	bool horizontal = false;
	vec2 ponto;
};

float interception(int y, Reta r){
	return (float)(y - r.b) / r.a;
}

template<class Tri>
std::vector<Pixel> scanline2(const Tri& P) {
	int xmin =  ceil(std::min({P[0][0], P[1][0], P[2][0]}));
	int xmax = floor(std::max({P[0][0], P[1][0], P[2][0]}));
	int ymin =  ceil(std::min({P[0][1], P[1][1], P[2][1]}));
	int ymax = floor(std::max({P[0][1], P[1][1], P[2][1]}));
	std::vector<Pixel> out;
	Reta reta[3]; 
	for (int i = 0; i < 3; i++) {
		int index1, index2;
		if (i < 2){
			index1 = i;
			index2 = i + 1;
		} else {
			index1 = 0;
			index2 = i;
		}
		reta[i].a = (P[index2][1] - P[index1][1]) / (P[index2][0] - P[index1][0]);
		reta[i].b = P[index1][1] - (reta[i].a * P[index1][0]);
	}	
	for (int y = ymin; y < ymax; y++) {
		std::vector<int> inter;
		for (int i = 0; i < 3; i++) {
			float a = interception(y, reta[i]);
			if (a > xmin && a < xmax) {
				inter.push_back(round(a));
			}
			std::cout << a << std::endl;
		}
		int min, max;
		if (inter. == 2) {
			min = ceil(std::min(inter[0], inter[1]));
			max = floor(std::max(inter[0], inter[1]));
		}
		else if (inter.size == 3) {
			min = ceil(std::min(inter[0], inter[1], inter[2]));
			max = floor(std::max(inter[0], inter[1], inter[2]));
		}
		for (int x = min; x < max; x++) {
			Pixel p;
			p.x = x;
			p.y = y;
			out.push_back(p);
		}
	}
	
	return out;
}

template<class Tri>
std::vector<Pixel> scanline(const Tri& P) {
	vec2 A = P[0];
	vec2 B = P[1];
	vec2 C = P[2];

	int xmin =  ceil(std::min({A[0], B[0], C[0]}));
	int xmax = floor(std::max({A[0], B[0], C[0]}));
	int ymin =  ceil(std::min({A[1], B[1], C[1]}));
	int ymax = floor(std::max({A[1], B[1], C[1]}));

	// Calcular o coeficiente angular e linear das 3 retas que formam o triangulo
	Reta reta[3]; 

	for (int i = 0; i < 3; i++) {
		
		int index1, index2;
		if (i < 2){
			index1 = i;
			index2 = i + 1;
		} else {
			index1 = 0;
			index2 = i;
		}
		// Estudo de casos das retas:

		// Caso #1: reta horizontal
		if (P[index1][1] - P[index2][1] == 0){
			reta[i].horizontal = true;
			reta[i].ponto = P[index1];
		}
		// Caso #2: reta vertical
		else if (P[index1][0] - P[index2][0] == 0){
			reta[i].vertical = true;
		}
		// Caso #3: demais casos
		else {
			// Equação da reta é: y = ax + b
			// Logo a = DeltaY / DeltaX
			// E b = y - ax
			reta[i].a = (P[index2][1] - P[index1][1]) / (P[index2][0] - P[index1][0]);
			reta[i].b = P[index1][1] - (reta[i].a * P[index1][0]);
			reta[i].ponto = P[index1];
		}
	}

	std::vector<Pixel> out;

	std::vector<float> inter;

	for (int y = ymin; y < ymax; y++){
		// Para cada y, calcular interseções
		for (int i = 0; i < 3; i++){
			float x = interception(y, reta[i]);
			if (x < xmax && x > xmin){
				inter.push_back(x);
			}
		}
		// Verificar qual a interseção é a max ou min
		int max, min;
		if (inter.size() == 2){
			max = floor(std::max(inter[0], inter[1]));
			min = ceil(std::min(inter[0], inter[1]));
		}
		else if (inter.size() == 3){
			max = floor(std::max(inter[0], inter[1], inter[2]));
			min = ceil(std::min(inter[0], inter[1], inter[2]));
		}

		std::cout << "y: " << y << std::endl;
		std::cout << "xmax: " << max << std::endl;
		std::cout << "xmin: " << min << std::endl;
		std::cout << std::endl;

		for (int x = min; x < max; x++){
			out.push_back({x, y});
		}
		inter.clear();
	}

	return out;
}

#endif
