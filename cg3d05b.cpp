#include "Graphics3D.h"
#include "ColorShader.h"
#include "SimpleShader.h"
#include "VertexUtils.h"
#include "transforms.h"

int main(){
	std::vector<Vec3Col> V = {
		{{0, 0, 0}, red},
		{{1, 0, 0}, red},
		{{1, 1, 0}, green},
		{{0, 1, 0}, green},
		{{0, 0, 1}, blue},
		{{1, 0, 1}, blue},
		{{1, 1, 1}, yellow},
		{{0, 1, 1}, yellow}
	};

	std::vector<unsigned int> indices = {
		4, 5, 7,   6, 7, 5,
		5, 1, 6,   2, 6, 1,
		6, 2, 7,   3, 7, 2
	};

	Elements<Triangles> T{indices};

	int w = 600, h = 600;
	mat4 Model = rotate_z(0.1);
	mat4 View = lookAt({1.2, 1.2, 1.2}, {0, 0, 0}, {0, 1, 0});
	float a = w/(float)h;
	mat4 Projection = perspective(60, a, 0.4, 2);
	mat4 M = Projection*View*Model;

	// ColorShader shader{ M };
	SimpleShader shader{ M, green};

	Graphics3D G{w, h};
	G.clear();	
	G.initZBuffer();
	G.draw(V, T, shader);
	G.savePNG("output.png");
}
