#include "Image.h"
#include "rasterization.h"
	
int main(){
	Image Img(600, 600);
	Img.fill(white);

	vec2 P[] = {
		{100, 300}, {200, 300},  {100, 200},
		{300, 200}, {400, 200},	{400, 300},
		{200, 400},{300, 300},{300, 450},
		{199.5,6.30645},{145,43},{199.5,31.5263},
		{150, 250}, {250, 200}, {350, 50}
	};

	auto drawSimple = [&](int i, int j, int k, Color c){
		vec2 T[3] = {P[i], P[j], P[k]};
		for(Pixel p : rasterizeTriangle(T))
			Img(p.x, p.y) = c;
	};

	auto drawScanLine = [&](int i, int j, int k, Color c){
		vec2 T[3] = {P[i], P[j], P[k]};
		for(Pixel p : scanline(T))
			Img(p.x, p.y) = c;
	};

	drawSimple(0, 1, 2, green);
	drawSimple(3, 4, 5, green);
	drawSimple(6, 7, 8, green);
	drawSimple(9, 10, 11, green);
	drawSimple(12, 13, 14, green);
	drawSimple(1, 1, 1, red); // degenerado - ponto
	drawSimple(3, 3, 4, red); // degenerado - reta
	drawSimple(6, 8, 8, red); // degenerado	- reta

	drawScanLine(0, 1, 2, blue);
	drawScanLine(3, 4, 5, blue);
	drawScanLine(6, 7, 8, blue);
	drawScanLine(9, 10, 11, blue);
	drawScanLine(12, 13, 14, blue);
	drawScanLine(1, 1, 1, orange); // degenerado
	drawScanLine(3, 3, 4, orange); // degenerado
	drawScanLine(6, 8, 8, orange); // degenerado



	Img.savePNG("output.png");
}
