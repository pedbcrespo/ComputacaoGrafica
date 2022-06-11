#include "Graphics3D.h"
#include "MarchingCubes.h"
#include "MixColorShader.h"
#include "VertexUtils.h"
#include "transforms.h"

struct Metaball{
	float a;
	float b;
	vec3 center;

	float operator()(vec3 P){
		float r = norm(P - center);

		return 
		    r >= b   ? 0.0f : 
		    r >= b/3 ? 3*a/2*pow(1 - r/b, 2): 
			           a*(1 - 3*r*r/(b*b));
	}
};

struct MetaballFigure{
	float T;
	std::vector<Metaball> metaballs;

	float operator()(float x, float y, float z){
		vec3 P = {x, y, z};

		float sum = 0;
		for (Metaball fi : metaballs)
			sum += fi(P);

		return T - sum;
	}
};

int main(){
	MetaballFigure figure;
	figure.T = 0.5;
    figure.metaballs = {
    		{1, 1, { 0, 0, 0}},
    		{1, 1, { 0, 1, 0}},
    		{1, 1, { 0, 0, 1}}, // cabeça
    		{1, 0.3, { 0, -0.5, 1}}, // fucinho
    		{1, 0.3, { 0, -0.6, 1}}, // reforço do fucinho
    		{1, 0.25, { 0, 0.4, 1.5}}, // orelha
    		{1, 0.25, { 0, -0.4, 1.5}}, // orelha
    		{1, 0.5, { 0.4, 0, -0.5}}, // perna
    		{1, 0.5, { -0.4, 0, -0.5}}, // perna
    		{1, 0.5, { 0.4, 1, -0.5}}, // perna
    		{1, 0.5, { -0.4, 1, -0.5}}, // perna
    		{1, 0.2, { 0, 1.3, 0.5}}, // cauda
        };

	vec3 pmin = {-2, -2, -2}; 
	vec3 pmax = { 2,  2,  2};
	
	std::vector<vec3> P = marchingCubes(figure, 50, 50, 50, pmin, pmax);
	
	MixColorShader shader;
	shader.pmin = pmin;
	shader.pmax = pmax;
	shader.C = {
		red, blue, green, yellow, 
		orange, magenta, cyan, purple
	};

	Triangles T{P.size()};

	int w = 600, h = 600;
	Graphics3D G{w, h};
	
	mat4 View = lookAt({2.5, 2.5, 1.5}, {0, 0, 0}, {0, 0, 1});
	float a = w/(float)h;
	mat4 Projection = perspective(45, a, 0.1, 100);

	int nframes = 5;
	for(int k = 0; k < nframes; k++){
		G.clear();
		G.initZBuffer();
		
		float theta = k*2*M_PI/(nframes-1);
		mat4 Model = rotate_z(theta);
		shader.M = Projection*View*Model;

		G.draw(P, T, shader);
		
		char filename[30];
		sprintf(filename, "tarefaSemana9/forma%03d.png", k);
		puts(filename);
		G.savePNG(filename);
	}
}
