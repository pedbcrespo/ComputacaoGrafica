#include "Image.h"
#include <algorithm>

int main(){
	Image F("odd.jpg");
	Image G("bob.jpg");

	float t = 0.3;

	int w = std::min(F.width(), G.width());
	int h = std::min(F.height(), G.height());
	Image H(w, h);
	for(int y = 0; y < h; y++)
		for(int x = 0; x < w; x++)
			H(x,y) = lerp(t, F(x,y), G(x,y));

	H.savePNG("output.png");
}
