#include "Graphics2D.h"

int main(){
	ClipRectangle R = {100, 200, 500, 400};

	std::vector<Vec2Col> P = {
		Vec2Col{{175.11, 437.82}, red},
		Vec2Col{{30.74, 329.70}, green},
		Vec2Col{{194.70, 314.24}, magenta},

		Vec2Col{{248.19, 285.13}, blue},
		Vec2Col{{350.00, 450.00}, orange},
		Vec2Col{{419.29, 410.49}, green},

		Vec2Col{{480.49, 452.08}, red},
		Vec2Col{{369.98, 295.82}, yellow},
		Vec2Col{{538.12, 116.40}, magenta},

		Vec2Col{{ 77.08, 289.29}, brown},
		Vec2Col{{211.94, 282.75}, purple},
		Vec2Col{{396.71, 133.63}, orange},

		Vec2Col{{160.85, 124.72}, red},
		Vec2Col{{223.23, 174.03}, red},
		Vec2Col{{237.49,  97.98}, cyan},

		Vec2Col{{307.60, 289.29}, blue},
		Vec2Col{{356.91, 327.31}, blue},
		Vec2Col{{355.72, 220.97}, cyan}
	};

	Triangles T{P.size()};

	std::vector<vec2> RP = {
		{R.x0, R.y0},  {R.x1, R.y0}, 
		{R.x1, R.y1},  {R.x0, R.y1}, 
	};
	LineLoop LR{4};

	Graphics2D G(600, 600);
	G.clear();

//	for(Triangle<Vec2Col> tri: assemble(T, P) )
	for(Triangle<Vec2Col> tri: clip(assemble(T, P), R))
		G.drawPrimitive(tri);

	G.draw(RP, LR, blue);
	
	G.savePNG("output.png");
}
