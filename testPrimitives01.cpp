#include "Primitives.h"
#include "rasterization.h"
#include "Image.h"

int main(){
	std::vector<vec2> P = {
		{20, 20},
		{80, 60},
		{220, 30},
		{350, 90},
		{70, 300},
		{320, 150}
	};
	
	Image Img(400, 400);
	Img.fill(white);

	//Lines L{P.size()};
	//LineStrip L{P.size()};
	LineLoop L{P.size()};
	
	for(Line<vec2> line: assemble(L, P))
		for(Pixel p : rasterizeLine(line))
			Img(p.x, p.y) = red;
	
	Img.savePNG("output.png");
}
