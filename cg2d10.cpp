#include "Graphics2D_v1.h"
#include "bezier.h"

int main(){
	//std::vector<vec2> P = loadCurve("borboleta.txt");
	std::vector<vec2> P = loadCurve("curvaC.txt");
	std::vector<vec2> Q = sample_cubic_bezier_spline(P, 20);

	LineStrip LP{P.size()};
	LineStrip LQ{Q.size()};
	
	Graphics2D G(400, 400);
	G.clear();
//	G.draw(P, LP, blue);
	G.draw(Q, LQ, red);
	G.savePNG("output.png");
}
